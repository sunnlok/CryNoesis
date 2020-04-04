#pragma once
#include <variant>

namespace Cry {
	namespace Ns
	{
		namespace LayoutDefinitions
		{
			using namespace Cry::Renderer::Pipeline;
			using namespace Cry::Renderer::Pipeline::Pass;
			using namespace Cry::Renderer::Shader;

			enum class In : uint64
			{
				Pos = 0,
				Color,
				Tex0,
				Tex1,
				Tex2,
				Coverage,
				Count
			};

			constexpr SInputElementDescription gElementDescriptions[] =
			{
				{ "POSITION", 0, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, EInputElementFormat::R8G8B8A8_UNORM, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 1, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 2, EInputElementFormat::R16G16B16A16_UNORM, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 3, EInputElementFormat::R32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 }
			};

			template<size_t size>
			using TDescrArray = std::array<SInputElementDescription, size>;

			using TDescVar = std::variant<TDescrArray<1>, TDescrArray<2>, TDescrArray<3>, TDescrArray<4>>;

			template<size_t size>
			struct TDesc
			{
				TDescrArray<size> descArray;

				template<typename ...Args>
				constexpr TDesc(Args... elements)
				{
					std::size_t i = 0;
					(void(descArray[i++] = gElementDescriptions[(uint64)elements]), ...);
				}		
			};

			template<In ...elements>
			constexpr auto GetElementDescription()
			{
				constexpr TDesc< sizeof...(elements)> desc(elements...);
				return desc.descArray;
			}

			enum class Extra : uint64
			{
				ST1 = (uint64)In::Count,
				Count
			};

			enum class Effect : uint64
			{
				Rgba = (uint64)Extra::Count,
				Mask,
				Path,
				Path_AA,
				SDF,
				SDF_LCD,
				Image_Opacity,
				Image_Shadow_V,
				Image_Shadow_H,
				Image_Blur_V,
				Image_Blur_H,
				Count
			};

			enum class Paint : uint64
			{
				Solid = (uint64)Effect::Count,
				Linear,
				Radial,
				Pattern,
				Count
			};

			enum class Gauss : uint64
			{
				Gauss_35_Tap = (uint64)Paint::Count,
				Gauss_63_Tap,
				Gauss_127_Tap,
				Count
			};

			const char* gMaskNames[] = {
				"Pos",
				"Color",
				"Tex0",
				"Tex1",
				"Tex2",
				"Coverage",
				"ST1",
				"Rgba",
				"Mask",
				"Path",
				"Path_AA",
				"SDF",
				"SDF_LCD",
				"Image_Opacity",
				"Image_Shadow_V",
				"Image_Shadow_H",
				"Image_Blur_V",
				"Image_Blur_H",
				"Solid",
				"Linear",
				"Radial",
				"Pattern",
				"Gauss_35_Tap",
				"Gauss_63_Tap",
				"Gauss_127_Tap"
			};

			template<class TBit>
			constexpr uint64 ToMask(TBit bit)
			{
				return 1ull << static_cast<uint64>(bit);
			}

			constexpr std::array<uint64, Noesis::Shader::Count> gShaderMasks = {
				ToMask(Effect::Rgba),ToMask(Effect::Mask),
				//Path_Solid
				 ToMask(Effect::Path) | ToMask(Paint::Solid),
				//Path_Linear
				 ToMask(Effect::Path) | ToMask(Paint::Linear),
				//Path_Radial
				 ToMask(Effect::Path) | ToMask(Paint::Radial),
				//Path_Pattern 
				 ToMask(Effect::Path) | ToMask(Paint::Pattern),
				//PathAA_Solid 
				 ToMask(Effect::Path_AA) | ToMask(Paint::Solid),
				//PathAA_Linear 
				 ToMask(Effect::Path_AA) | ToMask(Paint::Linear),
				//PathAA_Radial
				 ToMask(Effect::Path_AA) | ToMask(Paint::Radial),
				//PathAA_Pattern 
				 ToMask(Effect::Path_AA) | ToMask(Paint::Pattern),
				//SDF_Solid 
				 ToMask(Effect::SDF) | ToMask(Paint::Solid) | ToMask(Extra::ST1),
				//SDF_Linear
				 ToMask(Effect::SDF) | ToMask(Paint::Linear) | ToMask(Extra::ST1),
				//SDF_Radial
				 ToMask(Effect::SDF) | ToMask(Paint::Radial) | ToMask(Extra::ST1),
				//SDF_Pattern
				 ToMask(Effect::SDF) | ToMask(Paint::Pattern) | ToMask(Extra::ST1),
				//SDF_LCD_Solid
				 ToMask(Effect::SDF_LCD) | ToMask(Paint::Solid) | ToMask(Extra::ST1),
				//SDF_LCD_Linear
				 ToMask(Effect::SDF_LCD) | ToMask(Paint::Linear) | ToMask(Extra::ST1),
				//SDF_LCD_Radial 
				 ToMask(Effect::SDF_LCD) | ToMask(Paint::Radial) | ToMask(Extra::ST1),
				//SDF_LCD_Pattern 
				 ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern) | ToMask(Extra::ST1),
				//Image_Opacity_Solid 
				 ToMask(Effect::Image_Opacity) | ToMask(Paint::Solid),
				//Image_Opacity_Linear
				 ToMask(Effect::Image_Opacity) | ToMask(Paint::Linear),
				//Image_Opacity_Radial 
				 ToMask(Effect::Image_Opacity) | ToMask(Paint::Radial),
				//Image_Opacity_Pattern
				 ToMask(Effect::Image_Opacity) | ToMask(Paint::Pattern),
				//Image_Shadow35V 
				 ToMask(Effect::Image_Shadow_V) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Shadow63V 
				 ToMask(Effect::Image_Shadow_V) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Shadow127V
				 ToMask(Effect::Image_Shadow_V) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Shadow35H_Solid 
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Solid) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Shadow35H_Linear
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Linear) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Shadow35H_Radial
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Radial) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Shadow35H_Pattern
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Pattern) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Shadow63H_Solid
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Solid) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Shadow63H_Linear
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Linear) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Shadow63H_Radial 
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Radial) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Shadow63H_Pattern
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Pattern) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Shadow127H_Solid 
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Solid) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Shadow127H_Linear 
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Linear) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Shadow127H_Radial 
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Radial) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Shadow127H_Pattern
				 ToMask(Effect::Image_Shadow_H) | ToMask(Paint::Pattern) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Blur35V 
				 ToMask(Effect::Image_Blur_V) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Blur63V 
				 ToMask(Effect::Image_Blur_V) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Blur127V
				 ToMask(Effect::Image_Blur_V) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Blur35H_Solid 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Solid) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Blur35H_Linear
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Linear) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Blur35H_Radial 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Radial) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Blur35H_Pattern 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Pattern) | ToMask(Gauss::Gauss_35_Tap),
				//Image_Blur63H_Solid 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Solid) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Blur63H_Linear
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Linear) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Blur63H_Radial 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Radial) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Blur63H_Pattern 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Pattern) | ToMask(Gauss::Gauss_63_Tap),
				//Image_Blur127H_Solid 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Solid) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Blur127H_Linear
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Linear) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Blur127H_Radial 
				 ToMask(Effect::Image_Blur_H) | ToMask(Paint::Radial) | ToMask(Gauss::Gauss_127_Tap),
				//Image_Blur127H_Pattern
				ToMask(Effect::Image_Blur_H) | ToMask(Paint::Pattern) | ToMask(Gauss::Gauss_127_Tap),
			};

			template<Noesis::Shader::Enum shaderID, In... elements>
			constexpr uint64_t CreateMask()
			{
				return (ToMask(elements) | ... | gShaderMasks[shaderID]);
			}

			constexpr unsigned char elementSizesBits[] =
			{
				32 + 32,
				8 + 8 + 8 + 8,
				32 + 32,
				32 + 32,
				16 + 16 + 16 + 16,
				32,
				0
			};

			constexpr unsigned char GetElementSize(In elementType)
			{
				return elementSizesBits[(uint64)elementType] / 8;
			}

			template<In ...elements>
			constexpr unsigned char GetStride()
			{
				return (GetElementSize(elements) + ...);
			}
			
			struct SShaderDescInfo
			{
				Noesis::Shader	shader;
				TDescVar		descriptions;
				uint64			mask;
				uint8			stride;
			};

			using TDescInfoList = std::array<SShaderDescInfo, Noesis::Shader::Count>;

			template<Noesis::Shader::Enum shaderID, In... elements>
			constexpr SShaderDescInfo MakeInfo()
			{
				SShaderDescInfo info{
					shaderID
					, GetElementDescription<elements...>()
					, CreateMask<shaderID,elements...>()
					, GetStride<elements...>()
				};

				return info;
			}

			using namespace Noesis;

			constexpr TDescInfoList g_layoutInfoList = {
				MakeInfo<Shader::RGBA, In::Pos>(),
				MakeInfo<Shader::Mask, In::Pos>(),

				MakeInfo<Shader::Path_Solid, In::Pos, In::Color>(),
				MakeInfo<Shader::Path_Linear, In::Pos, In::Tex0>(),
				MakeInfo<Shader::Path_Radial, In::Pos, In::Tex0>(),
				MakeInfo<Shader::Path_Pattern, In::Pos, In::Tex0>(),

				MakeInfo<Shader::PathAA_Solid, In::Pos, In::Color, In::Coverage>(),
				MakeInfo<Shader::PathAA_Linear, In::Pos, In::Tex0, In::Coverage>(),
				MakeInfo<Shader::PathAA_Radial, In::Pos, In::Tex0, In::Coverage>(),
				MakeInfo<Shader::PathAA_Pattern, In::Pos, In::Tex0, In::Coverage>(),

				MakeInfo<Shader::SDF_Solid, In::Pos, In::Color, In::Tex1>(),
				MakeInfo<Shader::SDF_Linear, In::Pos, In::Tex0, In::Tex1>(),
				MakeInfo<Shader::SDF_Radial, In::Pos, In::Tex0, In::Tex1>(),
				MakeInfo<Shader::SDF_Pattern, In::Pos, In::Tex0, In::Tex1>(),

				MakeInfo<Shader::SDF_LCD_Solid, In::Pos, In::Color, In::Tex1>(),
				MakeInfo<Shader::SDF_LCD_Linear, In::Pos, In::Tex0, In::Tex1>(),
				MakeInfo<Shader::SDF_LCD_Radial, In::Pos, In::Tex0, In::Tex1>(),
				MakeInfo<Shader::SDF_LCD_Pattern, In::Pos, In::Tex0, In::Tex1>(),

				MakeInfo<Shader::Image_Opacity_Solid, In::Pos, In::Color, In::Tex1>(),
				MakeInfo<Shader::Image_Opacity_Linear, In::Pos, In::Tex0, In::Tex1>(),
				MakeInfo<Shader::Image_Opacity_Radial, In::Pos, In::Tex0, In::Tex1>(),
				MakeInfo<Shader::Image_Opacity_Pattern, In::Pos, In::Tex0, In::Tex1>(),

				MakeInfo<Shader::Image_Shadow35V, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow63V, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow127V, In::Pos, In::Color, In::Tex1, In::Tex2>(),

				MakeInfo<Shader::Image_Shadow35H_Solid, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow35H_Linear, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow35H_Radial, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow35H_Pattern, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),

				MakeInfo<Shader::Image_Shadow63H_Solid, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow63H_Linear,In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow63H_Radial, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow63H_Pattern, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),

				MakeInfo< Shader::Image_Shadow127H_Solid, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow127H_Linear, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow127H_Radial, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Shadow127H_Pattern, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),

				MakeInfo<Shader::Image_Blur35V, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur63V, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur127V, In::Pos, In::Color, In::Tex1, In::Tex2>(),

				MakeInfo<Shader::Image_Blur35H_Solid, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur35H_Linear, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur35H_Radial, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur35H_Pattern, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),

				MakeInfo<Shader::Image_Blur63H_Solid, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur63H_Linear, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur63H_Radial, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur63H_Pattern, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),

				MakeInfo<Shader::Image_Blur127H_Solid, In::Pos, In::Color, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur127H_Linear,In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur127H_Radial, In::Pos, In::Tex0, In::Tex1, In::Tex2>(),
				MakeInfo<Shader::Image_Blur127H_Pattern, In::Pos, In::Tex0, In::Tex1, In::Tex2>()
			};
		}
	}
}