#include <yaml-cpp/yaml.h>
#include <cassert>

#include "map.hpp"

Tile::Tile()
  : char_('#'),
    passable_(false),
    bg_colour_(0xaa, 0xaa, 0xaa),
    fg_colour_(0xff, 0xff, 0xff)
{
}

void TileMap::load_yaml(const char *fname)
{
  // this should all throw an exception 

  YAML::Node node = YAML::LoadFile(fname);

  assert(node.Type() == YAML::NodeType::Sequence);
  assert(node.IsSequence());

  for (const auto &tile_y : node) {
    assert(tile_y.IsMap());

    std::string name = tile_y["name"].as<std::string>();
    Tile tile;
    // FIXME: utf-8
    tile.char_ = tile_y["char"].as<std::string>()[0];
    tile.passable_ = tile_y["passable"].as<bool>();
    tile.fg_colour_ = Colour(tile_y["fg_colour"].as<unsigned>());
    tile.bg_colour_ = Colour(tile_y["bg_colour"].as<unsigned>());

    tiles_.insert(std::make_pair(name, tile));
  }
}

// ---------------------------------------------------------------------------

Map::Map()
  : width_(80),
    height_(50)
{
  tiles_.reset(new Tile[width_ * height_]);
  setup();
}

void Map::setup()
{
  for (int y = 1; y < height_ - 1; y++) {
    for (int x = 1; x < width_ - 1; x++) {
      tile_at(x, y).char_ = '.';
      tile_at(x, y).passable_ = '.';
      tile_at(x, y).bg_colour_ = Colour(0, 0, 0);
      tile_at(x, y).fg_colour_ = Colour(0x99, 0x99, 0x99);
    }
  }
}

void Map::load_yaml(const char *fname)
{
  YAML::Node yaml = YAML::LoadFile(fname);

}
