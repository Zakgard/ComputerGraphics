#pragma once

#include "Helpers.h"
#include <string>
#include "vector"

using namespace std;

class ShaderManager
{
public:

	ShaderManager() noexcept {};
	virtual ~ShaderManager() noexcept {};

	void CompileShaders(vector<string>& path);

	vector<ID3DBlob*> GetPixelShaderBC();
	vector<ID3DBlob*> GetVertexShadersBC();

private:
	vector<ID3DBlob*> sh_PixelBC;
	vector<ID3DBlob*> sh_VertexBC;
	vector<ID3DBlob*> sh_ErrorVertexBC;
	vector<ID3D11PixelShader*> sh_PixelShader;
	vector<ID3DBlob*> sh_ErrorPixelBC;
};



	

