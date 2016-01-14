#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <sstream>

#include "tinyformat.h"
#include "game.hpp"
#include "map.hpp"
#include "msg_log.hpp"
#include "unicode.hpp"

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
    tile.char_      = utf8_to_utf32(tile_y["char"].as<std::string>())[0];
    tile.passable_  = tile_y["passable"].as<bool>();
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
  dmsg(tfm::format("Loading map: '%s'...", fname));
  YAML::Node node = YAML::LoadFile(fname);

  assert(node.IsMap());

  const std::string &name = node["name"].as<std::string>();
  const int width = node["width"].as<int>();
  const int height = node["height"].as<int>();

  dmsg(tfm::format(" - map info: '%s' %dx%d", name, width, height));

  assert(width < 1000 && height < 1000);
  tiles_.reset(new Tile[width * height]);
  width_ = width;
  height_ = height;

  std::unordered_map<char32_t, Tile *> tile_lookup;
  TileMap *tl = get_tile_map();

  YAML::Node tile_map = node["tiles"];
  for (auto i = tile_map.begin(); i != tile_map.end(); ++i) {
    const std::string &key = i->first.as<std::string>();
    const std::string &val = i->second.as<std::string>();
    Tile *tile = NULL;

    auto ti = tl->tiles_.find(val);
    if (ti != tl->tiles_.end()) {
      tile = &ti->second;
    }

    std::u32string utf32 = utf8_to_utf32(key);
    tile_lookup[utf32[0]] = tile;
  }

  dmsg(" - tiles loaded");

  const std::string &map = node["map"].as<std::string>();
  std::istringstream iss(map);
  std::string line;
  int y = 0;

  while (iss >> line) {
    int x = -1;
    boost::algorithm::trim(line);
    std::u32string utf32 = utf8_to_utf32(line);
    for (char32_t c : utf32) {
      Tile *tile = NULL;
      auto ti = tile_lookup.find(c);
      if (ti != tile_lookup.end()) {
        tile = ti->second;
      } else {
      }

      x++;

      if (contains(x, y) && tile) {
        tile_at(x, y) = *tile;
      }
    }

    if (x >= 0)
      y++;
  }

  dmsg(" - map loaded");
}
