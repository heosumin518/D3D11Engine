#pragma once
#include "ResourceBase.h"

/// ��� ����.
/// ���̴��� ���̴��� ���Ҿ� �����ϴ� ���ڵ��� ���� ��� �ִ�.
/// ���̴��� ���õ� ���ڵ�� ���� ����.
/// ���̴��� �߰������� �ؽ�ó�� �Ǵ� ���ڵ��� �� �ִ� ��

class Shader;
class Texture;

class Material :
    public ResourceBase
{
    using Super = ResourceBase;
public:
    Material();
    virtual ~Material();

    shared_ptr<Shader> GetShader() { return _shader; }
    shared_ptr<Texture> GetTexture() { return _texture; }

    void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
    void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }

private:
    shared_ptr<Shader> _shader;

    // ���̴��� �ѱ�� �°� ���ڵ�
    shared_ptr<Texture> _texture;

};

