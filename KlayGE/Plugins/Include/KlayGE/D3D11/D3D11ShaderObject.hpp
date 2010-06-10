// D3D11ShaderObject.hpp
// KlayGE D3D11 shader对象类 头文件
// Ver 3.8.0
// 版权所有(C) 龚敏敏, 2009
// Homepage: http://www.klayge.org
//
// 3.8.0
// 初次建立 (2009.1.30)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11SHADEROBJECT_HPP
#define _D3D11SHADEROBJECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ShaderObject.hpp>
#include <KlayGE/MapVector.hpp>

#include <boost/function.hpp>

#include <D3D11Shader.h>

#include <KlayGE/D3D11/D3D11Typedefs.hpp>

namespace KlayGE
{
#ifdef KLAYGE_PLATFORM_WINDOWS
#pragma pack(push, 2)
#endif
	struct D3D11ShaderParameterHandle
	{
		uint32_t shader_type;

		D3D10_SHADER_VARIABLE_TYPE param_type;

		uint32_t cbuff;

		uint32_t offset;
		uint32_t elements;
		uint8_t rows;
		uint8_t columns;
	};
#ifdef KLAYGE_PLATFORM_WINDOWS
#pragma pack(pop)
#endif

	class D3D11ShaderObject : public ShaderObject
	{
	public:
		D3D11ShaderObject();

		std::string GenShaderText(RenderEffect const & effect, ShaderType cur_type) const;

		void SetShader(RenderEffect& effect, boost::shared_ptr<std::vector<uint32_t> > const & shader_desc_ids,
			uint32_t tech_index, uint32_t pass_index);
		ShaderObjectPtr Clone(RenderEffect& effect);

		void Bind();
		void Unbind();

		ID3D10BlobPtr const & VSCode() const
		{
			return vs_code_;
		}

		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> const & VSSignature() const
		{
			return vs_signature_;
		}

	private:
		struct parameter_bind_t
		{
			RenderEffectParameterPtr param;
			D3D11ShaderParameterHandle p_handle;
			boost::function<void()> func;
		};
		typedef std::vector<parameter_bind_t> parameter_binds_t;

		parameter_bind_t GetBindFunc(D3D11ShaderParameterHandle const & p_handle, RenderEffectParameterPtr const & param);

	private:
		boost::array<parameter_binds_t, ST_NumShaderTypes> param_binds_;
		boost::array<bool, ST_NumShaderTypes> is_shader_validate_;

		ID3D11VertexShaderPtr vertex_shader_;
		ID3D11PixelShaderPtr pixel_shader_;
		ID3D11GeometryShaderPtr geometry_shader_;
		ID3D11ComputeShaderPtr compute_shader_;
		ID3D11HullShaderPtr hull_shader_;
		ID3D11DomainShaderPtr domain_shader_;
		ID3D10BlobPtr vs_code_;

		boost::array<std::vector<ID3D11SamplerState*>, ST_NumShaderTypes> samplers_;
		boost::array<std::vector<ID3D11ShaderResourceView*>, ST_NumShaderTypes> srvs_;
		boost::array<std::vector<ID3D11UnorderedAccessView*>, ST_NumShaderTypes> uavs_;

		boost::array<std::vector<char>, ST_NumShaderTypes> dirty_;
		boost::array<std::vector<std::vector<uint8_t> >, ST_NumShaderTypes> cbufs_;
		boost::array<std::vector<ID3D11BufferPtr>, ST_NumShaderTypes> d3d_cbufs_;

		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> vs_signature_;
	};

	typedef boost::shared_ptr<D3D11ShaderObject> D3D11ShaderObjectPtr;
}

#endif			// _D3D11SHADEROBJECT_HPP
