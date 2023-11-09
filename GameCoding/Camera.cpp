#include "pch.h"
#include "Camera.h"

// static ���� �̱⿡ ����ٰ� ��ü�� ��üȭ�����ش�.
Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera()
	: Super(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	// ī�޶��� View ��� ���ϱ� ---------------------------------
	// ��� 1
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	// ��� 2
	//S_MatView = GetTransform()->GetWorldMatrix().Invert();
	// -----------------------------------------------------------

	if (_type == ProjectionType::Perspective)
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
	else
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
}
