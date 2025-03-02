#include "ShaderManager.h"
#include <filesystem>

using namespace std;


void ShaderManager::CompileShaders(vector<string>& pathes)
{
	if (pathes.size() < 1) return;
	string ext(".hlsl");

	//D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(1.0f, 1.0f, 1.0f, 1.0f)", nullptr, nullptr };
	//D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "SV_POSITION", "float4(1.0f, 1.0f, 1.0f, 1.0f)", nullptr, nullptr };

	for (const auto& path : pathes)
	{
		for (const auto& entry : filesystem::recursive_directory_iterator(path))
		{
			if (entry.path().extension() == ext) {

				sh_PixelBC.push_back(nullptr);
				sh_ErrorPixelBC.push_back(nullptr);

				sh_VertexBC.push_back(nullptr);
				sh_ErrorVertexBC.push_back(nullptr);
				
				D3DCompileFromFile(entry.path().c_str(),
					nullptr,
					nullptr,
					"PSMain", "ps_5_0",
					D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
					0,
					&sh_PixelBC[sh_PixelBC.size() - 1],
					&sh_ErrorPixelBC[sh_ErrorPixelBC.size() - 1]
				);

				D3DCompileFromFile(entry.path().c_str(),
					nullptr,
					nullptr,
					"VSMain", "vs_5_0",
					D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
					0,
					&sh_VertexBC[sh_VertexBC.size() - 1],
					&sh_ErrorVertexBC[sh_ErrorVertexBC.size() - 1]
				);
			}
		}
	}
}

vector<ID3DBlob*> ShaderManager::GetPixelShaderBC()
{
	return sh_PixelBC;
}

vector<ID3DBlob*> ShaderManager::GetVertexShadersBC()
{
	return sh_VertexBC;
}