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
	cdraw_assert(renderer && renderer->r && renderer->cdrawRendererPrint);
	return renderer->cdrawRendererPrint(renderer->r);
}

CDRAW_INL result_t cdrawRendererDisplay(cdrawRenderer const* const renderer)
{
	cdraw_assert(renderer && renderer->r && renderer->cdrawRendererDisplay);
	return renderer->cdrawRendererDisplay(renderer->r);
}


#endif // #if (!(defined _CDRAW_RENDERER_INL_) && (defined _CDRAW_RENDERER_H_))