/*
* Copyright 2023 Daniel S. Buckstein
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

/*
* cdrawRenderer.inl
* Inline definitions for renderer.
*/

#if (!(defined _CDRAW_RENDERER_INL_) && (defined _CDRAW_RENDERER_H_))
#define _CDRAW_RENDERER_INL_


CDRAW_INL result_t cdrawRendererPrint(cdrawRenderer const* const renderer)
{
	cdraw_assert(renderer && renderer->r && renderer->f && renderer->f->cdrawRendererPrint);
	return renderer->f->cdrawRendererPrint(renderer->r);
}

CDRAW_INL result_t cdrawRendererDisplay(cdrawRenderer const* const renderer)
{
	cdraw_assert(renderer && renderer->r && renderer->f && renderer->f->cdrawRendererDisplay);
	return renderer->f->cdrawRendererDisplay(renderer->r);
}

CDRAW_INL result_t cdrawRendererResize(cdrawRenderer const* const renderer, uint32_t const w_old, uint32_t const h_old, uint32_t const w_new, uint32_t const h_new)
{
	cdraw_assert(renderer && renderer->r && renderer->f && renderer->f->cdrawRendererResize);
	return renderer->f->cdrawRendererResize(renderer->r, w_old, h_old, w_new, h_new);
}

CDRAW_INL result_t cdrawRendererAttachWindow(cdrawRenderer const* const renderer, uint32_t const windowIndex, ptrk_t const p_data)
{
	cdraw_assert(renderer && renderer->r && renderer->f && renderer->f->cdrawRendererAttachWindow);
	return renderer->f->cdrawRendererAttachWindow(renderer->r, windowIndex, p_data);
}

CDRAW_INL result_t cdrawRendererDetachWindow(cdrawRenderer const* const renderer, uint32_t const windowIndex)
{
	cdraw_assert(renderer && renderer->r && renderer->f && renderer->f->cdrawRendererDetachWindow);
	return renderer->f->cdrawRendererDetachWindow(renderer->r, windowIndex);

}

CDRAW_INL result_t cdrawRendererWindowsSupported(cdrawRenderer const* const renderer, uint32_t* const count_out)
{
	cdraw_assert(renderer && renderer->r && renderer->f && renderer->f->cdrawRendererWindowsSupported);
	return renderer->f->cdrawRendererWindowsSupported(renderer->r, count_out);
}


#endif // #if (!(defined _CDRAW_RENDERER_INL_) && (defined _CDRAW_RENDERER_H_))