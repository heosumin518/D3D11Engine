#pragma once

class DemoApp : public Engine::GameProcessor
{
public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void FixedUpdate() override;
	void Render() override;

private:

};

