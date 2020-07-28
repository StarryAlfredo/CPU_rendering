#include "Color.h"

Vec3f GetFactor(BLEND_FACTOR& factor, TGAColor colorStr,TGAColor colorDst) {
	switch (factor) {
		case BLEND_FACTOR_ZERO:
			return Vec3f(0.0f, 0.0f, 0.0f);
		case BLEND_FACTOR_ONE:
			return Vec3f(0.0f, 0.0f, 0.0f);
		case BLEND_FACTOR_SRC_COLOR:
			return Vec3f(colorStr[0]/255.f, colorStr[1]/255.f, colorStr[2]/255.f);
		case BLEND_FACTOR_INV_STR_COLOR:
			return Vec3f(1.f - colorStr[0] / 255.f, 1.f - colorStr[1] / 255.f, 1.f - colorStr[2] / 255.f);
		case BLEND_FACTOR_SRC_ALPHA:
			return Vec3f(colorStr[3] / 255.f, colorStr[3] / 255.f, colorStr[3] / 255.f);
		case BLEND_FACTOR_INV_SRC_ALPHA:
			return Vec3f(1.f - colorStr[3] / 255.f, 1.f - colorStr[3] / 255.f, 1.f - colorStr[3] / 255.f);
		case BLEND_FACTOR_DEST_ALPHA:
			return Vec3f(colorDst[3] / 255.f, colorDst[3] / 255.f, colorDst[3] / 255.f);
		case BLEND_FACTOR_INV_DEST_ALPHA:
			return Vec3f(1.f - colorDst[3] / 255.f, 1.f - colorDst[3] / 255.f, 1.f - colorDst[3] / 255.f);
		case BLEND_FACTOR_DEST_COLOR:
			return Vec3f(colorDst[0] / 255.f, colorDst[1] / 255.f, colorDst[2] / 255.f);
		case BLEND_FACTOR_INV_DEST_COLOR:
			return Vec3f(1.f - colorDst[0] / 255.f, 1.f - colorDst[1] / 255.f, 1.f - colorDst[2] / 255.f);
		case BLEND_FACTOR_SRC_ALPHA_SAT:
			break;
		case BLEND_FACTOR_BLEND_FACTOR:
			break;
		case BLEND_FACTOR_INV_BLEND_FACTOR:
			break;
	}

}

TGAColor OMSetBlendState(TGAColor  src, TGAColor  dst, BLEND_OP op, BLEND_FACTOR factorSrc, BLEND_FACTOR factorDst)
{
	Vec3f factor_str;
	Vec3f factor_dst;
	TGAColor color;

	factor_str = GetFactor(factorSrc, src, dst);
	factor_dst = GetFactor(factorDst, src, dst);
	
	switch (op) {
		case BLEND_OP_ADD:
			color = src * factor_str + dst * factor_dst;
			break;
		case BLEND_OP_MAX:
			//
		case BLEND_OP_MIN:
			
		case BLEND_OP_REV_SUBTRACT:
			color = dst * factor_dst - src * factor_str;
		case BLEND_OP_SUBTRACT:
			color = src * factor_str - dst * factor_dst;
	}
	return color;
}
