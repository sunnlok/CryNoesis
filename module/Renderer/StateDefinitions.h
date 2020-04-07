#pragma once

namespace Cry::Ns::Renderer
{
	constexpr uint32 CreateDepthMaskDisabled()
	{
		using namespace Cry::Renderer::Stencil;

		constexpr auto mask =
			CSTENC_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENC_CCW_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_CCW_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENCOP_PASS(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_PASS(EStencilStateOp::FSS_STENCOP_KEEP);
		return mask;
	}

	constexpr uint32 CreateDepthMaskEqualKeep()
	{
		using namespace Cry::Renderer::Stencil;

		constexpr auto mask =
			CSTENC_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENC_CCW_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_CCW_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENCOP_PASS(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_PASS(EStencilStateOp::FSS_STENCOP_KEEP);
		return mask;
	}

	constexpr uint32 CreateDepthMaskEqualIncr()
	{
		using namespace Cry::Renderer::Stencil;

		constexpr auto mask =
			CSTENC_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENC_CCW_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_CCW_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENCOP_PASS(EStencilStateOp::FSS_STENCOP_INCR_WRAP)
			| CSTENCOP_CCW_PASS(EStencilStateOp::FSS_STENCOP_INCR_WRAP);
		return mask;
	}

	constexpr uint32 CreateDepthMaskEqualDecr()
	{
		using namespace Cry::Renderer::Stencil;

		constexpr auto mask =
			CSTENC_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENC_CCW_FUNC(EStencilStateFunction::FSS_STENCFUNC_EQUAL)
			| CSTENCOP_CCW_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENCOP_PASS(EStencilStateOp::FSS_STENCOP_DECR_WRAP)
			| CSTENCOP_CCW_PASS(EStencilStateOp::FSS_STENCOP_DECR_WRAP);
		return mask;
	}

	constexpr uint32 CreateDepthMaskZero()
	{
		using namespace Cry::Renderer::Stencil;

		constexpr auto mask =
			CSTENC_FUNC(EStencilStateFunction::FSS_STENCFUNC_ALWAYS)
			| CSTENCOP_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENC_CCW_FUNC(EStencilStateFunction::FSS_STENCFUNC_ALWAYS)
			| CSTENCOP_CCW_ZFAIL(EStencilStateOp::FSS_STENCOP_KEEP)
			| CSTENCOP_CCW_FAIL(EStencilStateOp::FSS_STENCOP_KEEP)

			| CSTENCOP_PASS(EStencilStateOp::FSS_STENCOP_ZERO)
			| CSTENCOP_CCW_PASS(EStencilStateOp::FSS_STENCOP_ZERO);
		return mask;
	}


	inline constexpr uint32 gDepthStateMasks[5]
	{
		GS_NODEPTHTEST | GS_DEPTHFUNC_NEVER,
		GS_NODEPTHTEST | GS_DEPTHFUNC_NEVER | GS_STENCIL,
		GS_NODEPTHTEST | GS_DEPTHFUNC_NEVER | GS_STENCIL,
		GS_NODEPTHTEST | GS_DEPTHFUNC_NEVER | GS_STENCIL,
		GS_NODEPTHTEST | GS_DEPTHFUNC_NEVER | GS_STENCIL
	};

	inline constexpr uint32 gDepthStencilMasks[5]
	{
		CreateDepthMaskDisabled(),
		CreateDepthMaskEqualKeep(),
		CreateDepthMaskEqualIncr(),
		CreateDepthMaskEqualDecr(),
		CreateDepthMaskZero()
	};


	inline constexpr uint32 gBlendStateSrc = GS_NOCOLMASK_NONE;
	inline constexpr uint32 gBlendStateSrcOver = GS_NOCOLMASK_NONE | GS_BLSRC_ONE | GS_BLDST_ONEMINUSSRCALPHA;
	inline constexpr uint32 gBlendStateSrcOverDual = GS_NOCOLMASK_NONE | GS_BLSRC_ONE | GS_BLDST_ONEMINUSSRCALPHA | GS_BLDST_ONEMINUSSRC1COLOR_A;
	inline constexpr uint32 gBlendColDisabled = GS_NOCOLMASK_RGBA;


	inline constexpr uint32 gBlendStateMask[4]
	{
		gBlendStateSrc,
		gBlendStateSrcOver,
		gBlendStateSrcOverDual,
		gBlendColDisabled
	};


	struct SSamplerAddress
	{
		Cry::Renderer::Sampler::EAddressMode u;
		Cry::Renderer::Sampler::EAddressMode v;
	};

	inline constexpr SSamplerAddress gSamplerAdresses[6]{
		{Cry::Renderer::Sampler::EAddressMode::Clamp,	Cry::Renderer::Sampler::EAddressMode::Clamp},
		{Cry::Renderer::Sampler::EAddressMode::Border,	Cry::Renderer::Sampler::EAddressMode::Border},
		{Cry::Renderer::Sampler::EAddressMode::Wrap,	Cry::Renderer::Sampler::EAddressMode::Wrap},
		{Cry::Renderer::Sampler::EAddressMode::Mirror,	Cry::Renderer::Sampler::EAddressMode::Wrap},
		{Cry::Renderer::Sampler::EAddressMode::Wrap,	Cry::Renderer::Sampler::EAddressMode::Mirror},
		{Cry::Renderer::Sampler::EAddressMode::Mirror,	Cry::Renderer::Sampler::EAddressMode::Mirror}
	};


	struct SSamplerFilters
	{
		uint8 min;
		uint8 mag;
		uint8 mip;
	};

	inline constexpr SSamplerFilters gSamplerFilter[6]
	{
		{FILTER_POINT, FILTER_POINT, FILTER_POINT},
		{FILTER_POINT, FILTER_POINT, FILTER_POINT},
		{FILTER_POINT, FILTER_POINT, FILTER_LINEAR},
		{FILTER_LINEAR, FILTER_LINEAR, FILTER_POINT},
		{FILTER_LINEAR, FILTER_LINEAR, FILTER_POINT},
		{FILTER_LINEAR, FILTER_LINEAR, FILTER_LINEAR}

	};
}