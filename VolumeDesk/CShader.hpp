

class CShader {

private:
    char *pShaderCode;
	int size;

public:
    CShader();
    ~CShader();
    
	int CShader::GetFileSize(const char *pName);
    int LoadFile(char *pFilePath);
    char * getShaderCode();
	int getFileSize();
};
