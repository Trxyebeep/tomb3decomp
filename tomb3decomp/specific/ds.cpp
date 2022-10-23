#include "../tomb3/pch.h"
#include "ds.h"

#define DS_Samples	ARRAY_(0x00632B50, LPDIRECTSOUNDBUFFER, [32])
#define DS_Buffers	ARRAY_(0x006326C8, LPDIRECTSOUNDBUFFER, [256])
#define DS_SamplesPlaying	ARRAY_(0x00632AD0, long, [32])
#define DS_SampleFrequencies	ARRAY_(0x006322B8, ulong, [256])

bool DS_IsChannelPlaying(long num)
{
	LPDIRECTSOUNDBUFFER buffer;
	ulong status;

	buffer = DS_Samples[num];

	if (buffer && SUCCEEDED(buffer->GetStatus(&status)))
	{
		if (status & DSBSTATUS_PLAYING)
			return 1;

		buffer->Release();
		DS_Samples[num] = 0;
	}

	return 0;
}

long DS_GetFreeChannel()
{
	for (int i = 0; i < 32; i++)
	{
		if (!DS_Samples[i])
			return i;
	}

	for (int i = 0; i < 32; i++)
	{
		if (!DS_IsChannelPlaying(i))
			return i;
	}

	return -1;
}

long DS_StartSample(long num, long volume, long pitch, long pan, ulong flags)
{
	LPDIRECTSOUNDBUFFER buffer;
	ulong fq;
	long channel;

	channel = DS_GetFreeChannel();

	if (channel < 0)
		return -1;

	if (FAILED(lpDirectSound->DuplicateSoundBuffer(DS_Buffers[num], &buffer)))
		return -2;

	fq = (pitch * DS_SampleFrequencies[num]) >> 16;

	if (fq < 100)
		fq = 100;

	if (fq > 100000)
		fq = 100000;

	if (FAILED(buffer->SetVolume(volume)) || FAILED(buffer->SetFrequency(fq)) || FAILED(buffer->SetPan(pan)) ||
		FAILED(buffer->SetCurrentPosition(0)) || FAILED(buffer->Play(0, 0, flags)))
		return -2;

	DS_SamplesPlaying[channel] = num;
	DS_Samples[channel] = buffer;
	return channel;
}

void DS_FreeAllSamples()
{
	if (App.DXConfig.sound)
	{
		for (int i = 0; i < 256; i++)
		{
			if (DS_Buffers[i])
			{
				DS_Buffers[i]->Release();
				DS_Buffers[i] = 0;
			}
		}
	}
}

bool DS_MakeSample(long num, LPWAVEFORMATEX fmt, LPVOID data, ulong bytes)
{
	DSBUFFERDESC desc;
	LPVOID pWrite;
	ulong aBytes;

	if (!App.DXConfig.sound || num > 256)
		return 0;

	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.lpwfxFormat = fmt;
	desc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	desc.dwBufferBytes = bytes;
	desc.dwReserved = 0;

	if (FAILED(lpDirectSound->CreateSoundBuffer(&desc, &DS_Buffers[num], 0)))
		return 0;

	if (FAILED(DS_Buffers[num]->Lock(0, bytes, &pWrite, &aBytes, 0, 0, 0)))
		return 0;

	memcpy(pWrite, data, aBytes);

	if (FAILED(DS_Buffers[num]->Unlock(pWrite, aBytes, 0, 0)))
		return 0;

	DS_SampleFrequencies[num] = fmt->nSamplesPerSec;
	return 1;
}

void DS_AdjustVolumeAndPan(long num, long volume, long pan)
{
	if (num >= 0 && DS_Samples[num])
	{
		DS_Samples[num]->SetVolume(volume);
		DS_Samples[num]->SetPan(pan);
	}
}

void inject_ds(bool replace)
{
	INJECT(0x00480740, DS_IsChannelPlaying, replace);
	INJECT(0x004808B0, DS_GetFreeChannel, replace);
	INJECT(0x00480790, DS_StartSample, replace);
	INJECT(0x00480600, DS_FreeAllSamples, replace);
	INJECT(0x00480630, DS_MakeSample, replace);
	INJECT(0x004808F0, DS_AdjustVolumeAndPan, replace);
}
