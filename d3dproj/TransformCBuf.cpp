#include "TransformCBuf.h"

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent)
	: vcbuf(gfx), parent(parent)
{
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{
	vcbuf.Update(gfx,
		parent.GetTransformXM() * gfx.GetProjection()
	);
	vcbuf.Bind(gfx);
}
