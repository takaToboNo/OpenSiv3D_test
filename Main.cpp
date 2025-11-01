# include <Siv3D.hpp>

void Main()
{
	Window::Resize(1280, 720);
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const Texture woodTexture{ U"example/texture/wood.jpg", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };

	constexpr double nearClip = 2.0;
	DebugCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 10, 16, -32 }, Vec3{0,0,0}, Vec3{0,1,0}, nearClip };

	while (System::Update())
	{
		camera.update(2.0);
		Graphics3D::SetCameraTransform(camera);

		// 3D 描画
		{
			const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };

			Plane{ 64 }.draw(uvChecker);

			{
				RasterizerState r = RasterizerState::Default3D;
				r.depthBias = 100;
				const ScopedRenderStates3D rasterizer{ r };

				Plane{ Vec3{ -16, 0, 0}, 12 }.draw(Linear::Palette::Red);
			}

			Plane{ Vec3{ 0, 0.0001, 0}, 12 }.draw(Linear::Palette::Red);

			Plane{ Vec3{ 16, 0.01, 0}, 12 }.draw(Linear::Palette::Red);
		}

		// 3D シーンを 2D シーンに描画
		{
			Graphics3D::Flush();
			renderTexture.resolve();
			Shader::LinearToScreen(renderTexture);
		}
	}
}
