#include "../tomb3/pch.h"
#include "hwinsert.h"
#include "../specific/hwrender.h"

#define	SetBufferPtrs(sort, info, nDrawType, pass)\
{\
	if(CurrentTLVertex - VertexBuffer > MAX_TLVERTICES - 32)\
		return;\
	\
	if (!App.lpZBuffer || !bAlphaTesting)\
	{\
		sort = sort3dptrbf;\
		info = info3dptrbf;\
		sort[2] = nPolyType;\
		surfacenumbf++;\
		sort3dptrbf += 3;\
		info3dptrbf += 5;\
	}\
	else if (pass || nDrawType == 14 || nDrawType == 13 || nDrawType == 12 || nDrawType == 16)\
	{\
		sort = sort3dptrfb;\
		info = info3dptrfb;\
		sort[2] = nPolyType;\
		surfacenumfb++;\
		sort3dptrfb += 3;\
		info3dptrfb += 5;\
	}\
	else\
	{\
		sort = sort3dptrbf;\
		info = info3dptrbf;\
		sort[2] = nPolyType;\
		surfacenumbf++;\
		sort3dptrbf += 3;\
		info3dptrbf += 5;\
	}\
}\

/*
	"pass" should only be true when the code looks like

	if (App.lpZBuffer && bAlphaTesting)
	{
		sort = sort3dptrfb;
		info = info3dptrfb;
		sort[2] = nPolyType;
		surfacenumfb++;
		sort3dptrfb += 3;
		info3dptrfb += 5;
	}
	else
	{
		sort = sort3dptrbf;
		info = info3dptrbf;
		sort[2] = nPolyType;
		surfacenumbf++;
		sort3dptrbf += 3;
		info3dptrbf += 5;
	}

	i.e when we only want to test zbuffer and alpha testing, and in all other cases go to fb- without falling back to bf if the drawtype doesn't match.
	*/

void HWI_InsertTrans8_Sorted(PHD_VBUF* buf, short shade)
{
	float z;
	long nPoints;
	char clip;

	clip = buf[0].clip | buf[1].clip | buf[2].clip | buf[3].clip | buf[4].clip | buf[5].clip | buf[6].clip | buf[7].clip;

	if (clip < 0 || (buf[0].clip & buf[1].clip & buf[2].clip & buf[3].clip & buf[4].clip & buf[5].clip & buf[6].clip & buf[7].clip) ||
		(buf[2].xs - buf[1].xs) * (buf->ys - buf[1].ys) - (buf[2].ys - buf[1].ys) * (buf->xs - buf[1].xs) < 0)
		return;

	for (int i = 0; i < 8; i++)
	{
		v_buffer[i].x = buf[i].xs;
		v_buffer[i].y = buf[i].ys;
		v_buffer[i].ooz = one / (buf[i].zv - 131072);
		v_buffer[i].vr = 0;
		v_buffer[i].vg = 0;
		v_buffer[i].vb = 0;
	}

	nPoints = 8;

	if (clip)
	{
		phd_leftfloat = (float)phd_winxmin;
		phd_topfloat = (float)phd_winymin;
		phd_rightfloat = float(phd_winxmin + phd_winwidth);
		phd_bottomfloat = float(phd_winymin + phd_winheight);
		nPoints = XYClipper(nPoints, v_buffer);
	}

	if (nPoints)
	{
		z = (buf[0].zv + buf[1].zv + buf[2].zv + buf[3].zv + buf[4].zv + buf[5].zv + buf[6].zv + buf[7].zv) * 0.125F - 131072;
		HWI_InsertPoly_Gouraud(nPoints, z, 0, 0, 0, 13);
	}
}

void SubdivideEdge(PHD_VBUF* v0, PHD_VBUF* v1, PHD_VBUF* dest)
{
	float zv;
	short r, g, b, r0, g0, b0, r1, g1, b1;

	dest->xv = (v1->xv + v0->xv) * 0.5F;
	dest->yv = (v0->yv + v1->yv) * 0.5F;
	dest->zv = (v0->zv + v1->zv) * 0.5F;

	if (dest->zv <= phd_znear)
		dest->clip = -128;
	else
	{
		zv = f_persp / dest->zv;
		dest->xs = dest->xv * zv + f_centerx;
		dest->ys = dest->yv * zv + f_centery;
		dest->ooz = zv * f_oneopersp;
		dest->clip = 0;

		if (dest->xs < phd_leftfloat)
			dest->clip++;
		else if (dest->xs > phd_rightfloat)
			dest->clip += 2;

		if (dest->ys < phd_topfloat)
			dest->clip += 4;
		else if (dest->ys > phd_bottomfloat)
			dest->clip += 8;
	}

	r0 = (v0->g >> 10) & 0x1F;
	r1 = (v1->g >> 10) & 0x1F;

	g0 = (v0->g >> 5) & 0x1F;
	g1 = (v1->g >> 5) & 0x1F;

	b0 = v0->g & 0x1F;
	b1 = v1->g & 0x1F;

	r = (r0 + r1) >> 1;
	g = (g0 + g1) >> 1;
	b = (b0 + b1) >> 1;
	dest->g = short(r << 10 | g << 5 | b);

	dest->u = (v0->u + v1->u) >> 1;
	dest->v = (v0->v + v1->v) >> 1;
}

void SubdivideGT4(PHD_VBUF* v1, PHD_VBUF* v2, PHD_VBUF* v3, PHD_VBUF* v4, PHDTEXTURESTRUCT* pTex, sort_type nSortType, ushort double_sided, long num)
{
	PHDTEXTURESTRUCT tex;
	PHD_VBUF v12;
	PHD_VBUF v34;
	PHD_VBUF v14;
	PHD_VBUF v23;
	PHD_VBUF v13;

	if (!num)
	{
		HWI_InsertGT4_Poly(v1, v2, v3, v4, pTex, nSortType, double_sided);
		return;
	}

	tex.drawtype = pTex->drawtype;
	tex.tpage = pTex->tpage;
	v1->u = pTex->u1;
	v1->v = pTex->v1;
	v2->u = pTex->u2;
	v2->v = pTex->v2;
	v3->u = pTex->u3;
	v3->v = pTex->v3;
	v4->u = pTex->u4;
	v4->v = pTex->v4;
	SubdivideEdge(v1, v2, &v12);
	SubdivideEdge(v3, v4, &v34);
	SubdivideEdge(v1, v4, &v14);
	SubdivideEdge(v2, v3, &v23);
	SubdivideEdge(v1, v3, &v13);
	tex.u1 = v1->u;
	tex.v1 = v1->v;
	tex.u2 = v12.u;
	tex.v2 = v12.v;
	tex.u3 = v13.u;
	tex.v3 = v13.v;
	tex.v4 = v14.v;
	tex.u4 = v14.u;
	SubdivideGT4(v1, &v12, &v13, &v14, &tex, nSortType, double_sided, num - 1);
	tex.u1 = v12.u;
	tex.v1 = v12.v;
	tex.u2 = v2->u;
	tex.v2 = v2->v;
	tex.u3 = v23.u;
	tex.v3 = v23.v;
	tex.u4 = v13.u;
	tex.v4 = v13.v;
	SubdivideGT4(&v12, v2, &v23, &v13, &tex, nSortType, double_sided, num - 1);
	tex.u1 = v13.u;
	tex.v1 = v13.v;
	tex.u2 = v23.u;
	tex.v2 = v23.v;
	tex.u3 = v3->u;
	tex.v3 = v3->v;
	tex.u4 = v34.u;
	tex.v4 = v34.v;
	SubdivideGT4(&v13, &v23, v3, &v34, &tex, nSortType, double_sided, num - 1);
	tex.u1 = v14.u;
	tex.v1 = v14.v;
	tex.u2 = v13.u;
	tex.v2 = v13.v;
	tex.u3 = v34.u;
	tex.v3 = v34.v;
	tex.u4 = v4->u;
	tex.v4 = v4->v;
	SubdivideGT4(&v14, &v13, &v34, v4, &tex, nSortType, double_sided, num - 1);
}

void SubdivideGT3(PHD_VBUF* v1, PHD_VBUF* v2, PHD_VBUF* v3, PHDTEXTURESTRUCT* pTex, sort_type nSortType, ushort double_sided, long num)
{
	PHD_VBUF v12;
	PHD_VBUF v23;
	PHD_VBUF v31;
	PHDTEXTURESTRUCT tex;

	if (!num)
	{
		HWI_InsertGT3_Poly(v1, v2, v3, pTex, &pTex->u1, &pTex->u2, &pTex->u3, nSortType, double_sided);
		return;
	}

	tex.drawtype = pTex->drawtype;
	tex.tpage = pTex->tpage;
	v1->u = pTex->u1;
	v1->v = pTex->v1;
	v2->u = pTex->u2;
	v2->v = pTex->v2;
	v3->u = pTex->u3;
	v3->v = pTex->v3;
	SubdivideEdge(v1, v2, &v12);
	SubdivideEdge(v2, v3, &v23);
	SubdivideEdge(v3, v1, &v31);
	tex.u1 = v1->u;
	tex.v1 = v1->v;
	tex.u2 = v12.u;
	tex.v2 = v12.v;
	tex.u3 = v31.u;
	tex.v3 = v31.v;
	SubdivideGT3(v1, &v12, &v31, &tex, nSortType, double_sided, num - 1);
	tex.u1 = v12.u;
	tex.v1 = v12.v;
	tex.u2 = v2->u;
	tex.v2 = v2->v;
	tex.u3 = v23.u;
	tex.v3 = v23.v;
	SubdivideGT3(&v12, v2, &v23, &tex, nSortType, double_sided, num - 1);
	tex.u1 = v31.u;
	tex.v1 = v31.v;
	tex.u2 = v12.u;
	tex.v2 = v12.v;
	tex.u3 = v23.u;
	tex.v3 = v23.v;
	tex.u4 = v3->u;
	tex.v4 = v3->v;
	SubdivideGT4(&v31, &v12, &v23, v3, &tex, nSortType, double_sided, num - 1);
}

void HWI_InsertGT4_Sorted(PHD_VBUF* v1, PHD_VBUF* v2, PHD_VBUF* v3, PHD_VBUF* v4, PHDTEXTURESTRUCT* pTex, sort_type nSortType, ushort double_sided)
{
	float zv;

	if (App.lpZBuffer || nPolyType != 3 && nPolyType != 4)
	{
		HWI_InsertGT4_Poly(v1, v2, v3, v4, pTex, nSortType, double_sided);
		return;
	}

	zv = v1->zv;

	if (zv < v2->zv)
		zv = v2->zv;

	if (zv < v3->zv)
		zv = v3->zv;

	if (zv < v4->zv)
		zv = v4->zv;

	if (zv < 0x1F40000)
		SubdivideGT4(v1, v2, v3, v4, pTex, nSortType, double_sided, 2);
	else if (zv < 0x36B0000)
		SubdivideGT4(v1, v2, v3, v4, pTex, nSortType, double_sided, 1);
	else
		HWI_InsertGT4_Poly(v1, v2, v3, v4, pTex, nSortType, double_sided);
}

void HWI_InsertGT3_Sorted(PHD_VBUF* v1, PHD_VBUF* v2, PHD_VBUF* v3, PHDTEXTURESTRUCT* pTex, ushort* uv1, ushort* uv2, ushort* uv3, sort_type nSortType, ushort double_sided)
{
	float zv;

	if (App.lpZBuffer || nPolyType != 3 && nPolyType != 4)
	{
		HWI_InsertGT3_Poly(v1, v2, v3, pTex, &pTex->u1, &pTex->u2, &pTex->u3, nSortType, double_sided);
		return;
	}

	zv = v1->zv;

	if (zv < v2->zv)
		zv = v2->zv;

	if (zv < v3->zv)
		zv = v3->zv;

	if (zv < 0x1F40000)
		SubdivideGT3(v1, v2, v3, pTex, nSortType, double_sided, 2);
	else if (zv < 0x36B0000)
		SubdivideGT3(v1, v2, v3, pTex, nSortType, double_sided, 1);
	else
		HWI_InsertGT3_Poly(v1, v2, v3, pTex, &pTex->u1, &pTex->u2, &pTex->u3, nSortType, double_sided);
}

void HWI_InsertTransQuad_Sorted(long x, long y, long w, long h, long z)
{
	D3DTLVERTEX* v;
	long* sort;
	short* info;
	float zv;

	SetBufferPtrs(sort, info, 0, 1);
	sort[0] = (long)info;
	sort[1] = z;
	info[0] = 13;
	info[1] = 0;
	info[2] = 4;

	v = CurrentTLVertex;
	*((D3DTLVERTEX**)(info + 3)) = CurrentTLVertex;
	zv = one / (float)z;

	v[0].sx = (float)x;
	v[0].sy = (float)y;
	v[0].sz = f_a - zv * f_boo;
	v[0].rhw = zv;
	v[0].color = 0x50003FFF;

	v[1].sx = float(x + w);
	v[1].sy = (float)y;
	v[1].sz = f_a - zv * f_boo;
	v[1].rhw = zv;
	v[1].color = 0x50003FFF;

	v[2].sx = float(x + w);
	v[2].sy = float(y + h);
	v[2].sz = f_a - zv * f_boo;
	v[2].rhw = zv;
	v[2].color = 0x50003F1F;

	v[3].sx = (float)x;
	v[3].sy = float(y + h);
	v[3].sz = f_a - zv * f_boo;
	v[3].rhw = zv;
	v[3].color = 0x50003F1F;
	CurrentTLVertex = v + 4;
}

void InitUVTable()
{
	for (int i = 0; i < 65536; i++)
		UVTable[i] = float(i + 1) * (1.0F / 65536.0F);
}

long GETR(long col)
{
	long r;

	r = ColorTable[(col >> 7) & 0xF8];

	if (bBlueEffect)
		return (128 * r) >> 8;

	return r;
}

long GETG(long col)
{
	long g;

	g = ColorTable[(col >> 2) & 0xF8];

	if (bBlueEffect)
		return (224 * g) >> 8;

	return g;
}

long GETB(long col)
{
	long b;

	b = ColorTable[(col & 0x1F) << 3];
	return b;
}

void PHD_VBUF_To_D3DTLVTX(PHD_VBUF* phdV, D3DTLVERTEX* v, ushort* uv)
{
	long r, g, b;

	v->sx = phdV->xs;
	v->sy = phdV->ys;
	v->sz = f_a - f_boo * phdV->ooz;
	v->rhw = phdV->ooz;
	r = GETR(phdV->g);
	g = GETG(phdV->g);
	b = GETB(phdV->g);
	v->color = GlobalAlpha | (r << 16) | (g << 8) | b;
	v->tu = UVTable[uv[0]];
	v->tv = UVTable[uv[1]];
	v->specular = 0;
}

void PHD_VBUF_To_VERTEX_INFO(PHD_VBUF* phdV, VERTEX_INFO* v)
{
	v->x = phdV->xs;
	v->y = phdV->ys;
	v->ooz = phdV->ooz;
	v->vr = GETR(phdV->g);
	v->vg = GETG(phdV->g);
	v->vb = GETB(phdV->g);
}

long visible_zclip(PHD_VBUF* v0, PHD_VBUF* v1, PHD_VBUF* v2)
{
	return (v2->xv * v0->zv - v0->xv * v2->zv) * v1->yv +
		(v0->xv * v2->yv - v2->xv * v0->yv) * v1->zv +
		(v0->yv * v2->zv - v2->yv * v0->zv) * v1->xv < 0;
}

long FindBucket(DXTEXTURE* TPage)
{
	TEXTUREBUCKET* bucket;
	long nVtx, fullest;

	if (nDrawnVerts > 2700)
		return -1;

	for (int i = 0; i < 6; i++)
	{
		bucket = &Buckets[i];

		if (bucket->TPage == TPage && bucket->nVtx < 256)
			return i;

		if (bucket->nVtx > 256)
		{
			HWR_EnableZBuffer(1, 1);
			HWR_SetCurrentTexture(bucket->TPage);
			DrawPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, bucket->vtx, bucket->nVtx, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS);
			bucket->TPage = TPage;
			bucket->nVtx = 0;
			return i;
		}
	}

	nVtx = 0;
	fullest = 0;

	for (int i = 0; i < 6; i++)
	{
		bucket = &Buckets[i];

		if (bucket->TPage == (DXTEXTURE*)-1)
		{
			bucket->TPage = TPage;
			return i;
		}

		if (bucket->nVtx > nVtx)
		{
			nVtx = bucket->nVtx;
			fullest = i;
		}
	}

	bucket = &Buckets[fullest];
	HWR_EnableZBuffer(1, 1);
	HWR_SetCurrentTexture(bucket->TPage);
	DrawPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, bucket->vtx, bucket->nVtx, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS);
	bucket->TPage = TPage;
	bucket->nVtx = 0;
	return fullest;
}

void DrawBuckets()
{
	TEXTUREBUCKET* bucket;

	for (int i = 0; i < 6; i++)
	{
		bucket = &Buckets[i];

		if (bucket->nVtx)
		{
			HWR_SetCurrentTexture(bucket->TPage);
			DrawPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, bucket->vtx, bucket->nVtx, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS);
		}
	}
}

void HWI_InsertClippedPoly_Textured(long nPoints, float zdepth, long nDrawType, long nTPage)
{
	VERTEX_INFO* vtxbuf;
	D3DTLVERTEX* v;
	TEXTUREBUCKET* bucket;
	long* sort;
	short* info;
	float z;
	long nBucket, nVtx;

	vtxbuf = v_buffer;

	if (App.lpZBuffer && nDrawType != 14 && nDrawType != 10)
	{
		nBucket = FindBucket(TPages[nTPage]);

		if (nBucket == -1)
			return;

		for (int i = 0; i < 3; i++, vtxbuf++)
		{
			bucket = &Buckets[nBucket];
			nVtx = bucket->nVtx;
			v = &bucket->vtx[nVtx];
			v->sx = vtxbuf->x;
			v->sy = vtxbuf->y;
			v->sz = f_a - f_boo * vtxbuf->ooz;
			v->rhw = vtxbuf->ooz;
			v->color = GlobalAlpha | (vtxbuf->vr << 16) | (vtxbuf->vg << 8) | vtxbuf->vb;
			v->specular = 0;
			z = (1.0F / 65536.0F) / vtxbuf->ooz;
			v->tu = vtxbuf->u * z;
			v->tv = vtxbuf->v * z;
			bucket->nVtx++;
		}

		vtxbuf--;
		nPoints -= 3;
		nDrawnVerts++;

		if (nPoints)
		{
			nDrawnVerts += nPoints;

			for (int i = nPoints; i; i--)
			{
				bucket = &Buckets[nBucket];
				nVtx = bucket->nVtx;

				v = &bucket->vtx[nVtx];
				v->sx = v_buffer->x;
				v->sy = v_buffer->y;
				v->sz = f_a - f_boo * v_buffer->ooz;
				v->rhw = v_buffer->ooz;
				v->color = GlobalAlpha | (v_buffer->vr << 16) | (v_buffer->vg << 8) | v_buffer->vb;
				v->specular = 0;
				z = (1.0F / 65536.0F) / v_buffer->ooz;
				v->tu = v_buffer->u * z;
				v->tv = v_buffer->v * z;

				v = &bucket->vtx[nVtx + 1];
				v->sx = vtxbuf->x;
				v->sy = vtxbuf->y;
				v->sz = f_a - f_boo * vtxbuf->ooz;
				v->rhw = vtxbuf->ooz;
				v->color = GlobalAlpha | (vtxbuf->vr << 16) | (vtxbuf->vg << 8) | vtxbuf->vb;
				v->specular = 0;
				z = (1.0F / 65536.0F) / vtxbuf->ooz;
				v->tu = vtxbuf->u * z;
				v->tv = vtxbuf->v * z;

				vtxbuf++;
				v = &bucket->vtx[nVtx + 2];
				v->sx = vtxbuf->x;
				v->sy = vtxbuf->y;
				v->sz = f_a - f_boo * vtxbuf->ooz;
				v->rhw = vtxbuf->ooz;
				v->color = GlobalAlpha | (vtxbuf->vr << 16) | (vtxbuf->vg << 8) | vtxbuf->vb;
				v->specular = 0;
				z = (1.0F / 65536.0F) / vtxbuf->ooz;
				v->tu = vtxbuf->u * z;
				v->tv = vtxbuf->v * z;

				bucket->nVtx += 3;
			}
		}
	}
	else
	{
		SetBufferPtrs(sort, info, nDrawType, 0);
		sort[0] = (long)info;
		sort[1] = (long)zdepth;
		info[0] = (short)nDrawType;
		info[1] = (short)nTPage;
		info[2] = (short)nPoints;
		v = CurrentTLVertex;
		*((D3DTLVERTEX**)(info + 3)) = v;

		for (int i = nPoints; i; i--, v++, vtxbuf++)
		{
			v->sx = vtxbuf->x;
			v->sy = vtxbuf->y;
			v->sz = f_a - f_boo * vtxbuf->ooz;
			v->rhw = vtxbuf->ooz;
			v->color = GlobalAlpha | (vtxbuf->vr << 16) | (vtxbuf->vg << 8) | vtxbuf->vb;
			v->specular = 0;
			z = (1.0F / 65536.0F) / vtxbuf->ooz;
			v->tu = vtxbuf->u * z;
			v->tv = vtxbuf->v * z;
		}

		CurrentTLVertex = v;
	}
}

void inject_hwinsert(bool replace)
{
	INJECT(0x0040A850, HWI_InsertTrans8_Sorted, replace);
	INJECT(0x00406880, SubdivideEdge, replace);
	INJECT(0x004069E0, SubdivideGT4, replace);
	INJECT(0x00406D50, SubdivideGT3, replace);
	INJECT(0x00406FA0, HWI_InsertGT4_Sorted, replace);
	INJECT(0x00405980, HWI_InsertGT3_Sorted, replace);
	INJECT(0x0040A690, HWI_InsertTransQuad_Sorted, replace);
	INJECT(0x00405220, InitUVTable, replace);
	INJECT(0x00406750, GETR, replace);
	INJECT(0x00406780, GETG, replace);
	INJECT(0x004067B0, PHD_VBUF_To_D3DTLVTX, replace);
	INJECT(0x004094D0, PHD_VBUF_To_VERTEX_INFO, replace);
	INJECT(0x004053E0, visible_zclip, replace);
	INJECT(0x00405270, FindBucket, replace);
	INJECT(0x004053A0, DrawBuckets, replace);
	INJECT(0x00405450, HWI_InsertClippedPoly_Textured, replace);
}
