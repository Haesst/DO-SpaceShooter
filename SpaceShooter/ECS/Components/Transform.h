#pragma once

#include "../Base/Component.h"
#include "../../Utils/Vector2D.h"

namespace ECS
{
	struct Transform : public Component
	{
		Vector2D Position = Vector2D().Zero();
		Vector2D Scale = Vector2D().One();
		float Rotation = 0.0f;

		Transform() = default;

		Transform(float x, float y)
			: Position(Vector2D(x, y)), Scale(Vector2D().One())
		{}

		Transform(float x, float y, float scaleX, float scaleY)
			: Position(Vector2D(x, y)), Scale(Vector2D(scaleX, scaleY))
		{}

		Transform(float x, float y, float scaleX, float scaleY, float rotation)
			: Position(Vector2D(x, y)), Scale(Vector2D(scaleX, scaleY)), Rotation(rotation)
		{}

		void Translate(Vector2D v)
		{
			Position += v;
		}

		virtual ~Transform() = default;
	};
}