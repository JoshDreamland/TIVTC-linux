/*
**                    TIVTC v1.0.5 for Avisynth 2.5.x
**
**   TIVTC includes a field matching filter (TFM) and a decimation
**   filter (TDecimate) which can be used together to achieve an
**   IVTC or for other uses. TIVTC currently supports YV12 and 
**   YUY2 colorspaces.
**   
**   Copyright (C) 2004-2008 Kevin Stone
**
**   This program is free software; you can redistribute it and/or modify
**   it under the terms of the GNU General Public License as published by
**   the Free Software Foundation; either version 2 of the License, or
**   (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**   GNU General Public License for more details.
**
**   You should have received a copy of the GNU General Public License
**   along with this program; if not, write to the Free Software
**   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <math.h>
#define TFMPP_INCLUDED
#ifndef TFM_INCLUDED
#include "TFM.h"
#endif
#ifdef VERSION
#undef VERSION
#endif
#define VERSION "v1.0.2"

class TFMPP : public GenericVideoFilter
{
private:
	const char *ovr;
	int PP, PPS, nfrms, mthresh, mthreshS, setArraySize;
	int *setArray, opt;
	bool display, uC2, usehints;
	char buf[512];
	PClip clip2;
	PlanarFrame *mmask;
	void buildMotionMask(PVideoFrame &prv, PVideoFrame &src, PVideoFrame &nxt, 
		PlanarFrame *mask, int use, int np, IScriptEnvironment *env);
	void BlendDeint(PVideoFrame &src, PlanarFrame *mask, PVideoFrame &dst, 
		bool nomask, int np, IScriptEnvironment *env);
	void maskClip2(PVideoFrame &src, PVideoFrame &deint, PlanarFrame *mask, 
		PVideoFrame &dst, int np, IScriptEnvironment *env);
	void maskClip2_MMX(const unsigned char *srcp, const unsigned char *dntp, 
	  const unsigned char *maskp, unsigned char *dstp, int src_pitch, int dnt_pitch,
	  int msk_pitch, int dst_pitch, int width, int height);
	void maskClip2_SSE2(const unsigned char *srcp, const unsigned char *dntp, 
	  const unsigned char *maskp, unsigned char *dstp, int src_pitch, int dnt_pitch,
	  int msk_pitch, int dst_pitch, int width, int height);
	void putHint(PVideoFrame &dst, int field, unsigned int hint);
	bool getHint(PVideoFrame &src, int &field, bool &combed, unsigned int &hint);
	void getSetOvr(int n);
	void copyFrame(PVideoFrame &dst, PVideoFrame &src, IScriptEnvironment *env, int np);
	void denoiseYUY2(PlanarFrame *mask);
	void denoiseYV12(PlanarFrame *mask);
	void linkYUY2(PlanarFrame *mask);
	void linkYV12(PlanarFrame *mask);
	void destroyHint(PVideoFrame &dst, unsigned int hint);
	void CubicDeint(PVideoFrame &src, PlanarFrame *mask, PVideoFrame &dst, bool nomask, 
					   int field, int np, IScriptEnvironment *env);
	unsigned char cubicInt(unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4);
	void writeDisplay(PVideoFrame &dst, int np, int n, int field);
	void elaDeint(PVideoFrame &dst, PlanarFrame *mask, PVideoFrame &src, bool nomask, int field, int np);
	void elaDeintYV12(PVideoFrame &dst, PlanarFrame *mask, PVideoFrame &src, bool nomask, int field);
	void elaDeintYUY2(PVideoFrame &dst, PlanarFrame *mask, PVideoFrame &src, bool nomask, int field);
	void blendDeint_MMX(const unsigned char *srcp, unsigned char *dstp, int src_pitch,
		int dst_pitch, int width, int height);
	void blendDeint_SSE2(const unsigned char *srcp, unsigned char *dstp, int src_pitch,
		int dst_pitch, int width, int height);
	void blendDeintMask_MMX(const unsigned char *srcp, unsigned char *dstp, 
		const unsigned char *maskp, int src_pitch, int dst_pitch, int msk_pitch, 
		int width, int height);
	void blendDeintMask_SSE2(const unsigned char *srcp, unsigned char *dstp, 
		const unsigned char *maskp, int src_pitch, int dst_pitch, int msk_pitch, 
		int width, int height);
	void cubicDeint_SSE2(const unsigned char *srcp, unsigned char *dstp, int src_pitch,
		int dst_pitch, int width, int height);
	void cubicDeint_MMX(const unsigned char *srcp, unsigned char *dstp, int src_pitch,
		int dst_pitch, int width, int height);
	void cubicDeintMask_SSE2(const unsigned char *srcp, unsigned char *dstp, 
		const unsigned char *maskp, int src_pitch, int dst_pitch, int msk_pitch,
		int width, int height);
	void cubicDeintMask_MMX(const unsigned char *srcp, unsigned char *dstp, 
		const unsigned char *maskp, int src_pitch, int dst_pitch, int msk_pitch, 
		int width, int height);
	void copyField(PVideoFrame &dst, PVideoFrame &src, IScriptEnvironment *env, int np,
		int field);
	void buildMotionMask1_SSE2(const unsigned char *srcp1, const unsigned char *srcp2,
		unsigned char *dstp, int s1_pitch, int s2_pitch, int dst_pitch, int width, int height, long cpu);
	void buildMotionMask1_MMX(const unsigned char *srcp1, const unsigned char *srcp2,
		unsigned char *dstp, int s1_pitch, int s2_pitch, int dst_pitch, int width, int height, long cpu);
	void buildMotionMask2_SSE2(const unsigned char *srcp1, const unsigned char *srcp2,
		const unsigned char *srcp3, unsigned char *dstp, int s1_pitch, int s2_pitch, 
		int s3_pitch, int dst_pitch, int width, int height, long cpu);
	void buildMotionMask2_MMX(const unsigned char *srcp1, const unsigned char *srcp2,
		const unsigned char *srcp3, unsigned char *dstp, int s1_pitch, int s2_pitch, 
		int s3_pitch, int dst_pitch, int width, int height, long cpu);

public:
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
    TFMPP(PClip _child, int _PP, int _mthresh, const char* _ovr, bool _display, PClip _clip2, 
		bool _usehints, int _opt, IScriptEnvironment* env);
	~TFMPP();
};
