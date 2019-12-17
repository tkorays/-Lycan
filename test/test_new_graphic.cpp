#include "../include/lycan/lycan.h"
#include "../include/lycan/graphic.h"
#include "../include/lycan/imgproc.h"
#include "../include/lycan/sdlrender.h"

using namespace lycan;

int main() {
    lycan::Matrix<int> mat(600, 400);
    lycan::imgproc::fill(mat, lycan::Rect<int>(100, 100, 300, 300), lycan::Rgba(255, 0, 0, 0).argb<int>());
    lycan::imgproc::fill(mat, lycan::Rect<int>(200, 200, 400, 400), lycan::Rgba(0, 255, 0, 0).argb<int>());
    //auto ret = lycan::imgproc::move(mat, lycan::Position<int32_t>(-150, 150), lycan::Rgba(0, 0, 255, 0).argb<int>());
    //auto ret = lycan::imgproc::mirrorV(lycan::imgproc::mirrorH(mat));
    //auto ret = lycan::imgproc::transpose(mat);
    //auto ret = lycan::imgproc::scale(mat, 2, 2);
    auto ret = lycan::imgproc::rotate<int>(lycan::imgproc::scale<int>(mat, 0.5, 1.5), 20.0f, lycan::Rgba(0, 0, 0, 255).argb<int>());

    lycan::ext::SDLRender::Init();
    lycan::ext::SDLRender render("img");

    while(render.Wait()) {
        render.ShowImage(ret.ptrData(), ret.cols(), ret.rows(), ret.cols()*4, lycan::ext::SDLRender::FMT_RGBA32);
        if(render.LoopEnd()) break;
    }
    return 0;
}