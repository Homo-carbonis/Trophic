#include "trophic.hpp"
int main(int argc, char *argv[])
{
  World world;
  png::image<png::rgba_pixel,png::solid_pixel_buffer<png::rgba_pixel>> image("sprite.png");
  Texture texture({image.get_width(), image.get_height(), 1},
      (uint8_t*)image.get_pixbuf().get_bytes().data());
  Sprite& sprite=world.newSprite(texture);
  sprite.newInstance();
  world.run();
}
