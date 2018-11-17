//  SuperTux
//  Copyright (C) 2015 Hume2 <teratux.mail@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_SUPERTUX_EDITOR_EDITOR_HPP
#define HEADER_SUPERTUX_EDITOR_EDITOR_HPP

#include <string>

#include "editor/input_center.hpp"
#include "editor/input_gui.hpp"
#include "editor/layers_gui.hpp"
#include "editor/scroller.hpp"
#include "supertux/screen.hpp"
#include "supertux/world.hpp"
#include "util/currenton.hpp"
#include "util/file_system.hpp"
#include "util/log.hpp"
#include "video/surface_ptr.hpp"

class GameObject;
class Level;
class ObjectGroup;
class Path;
class Savegame;
class Sector;
class TileSet;
class World;

class Editor final : public Screen,
                     public Currenton<Editor>
{
public:
  static bool is_active();

public:
  Editor();

  virtual void draw(Compositor&) override;
  virtual void update(float dt_sec, const Controller& controller) override;

  virtual void setup() override;
  virtual void leave() override;

  void event(SDL_Event& ev);
  void resize();

  void disable_keyboard() { enabled = false; }

  Level* get_level() const { return level.get(); }

  void set_world(std::unique_ptr<World> w);
  World* get_world() const { return world.get(); }

  TileSet* get_tileset() const { return tileset; }
  TileSelection* get_tiles() const { return tileselect.m_tiles.get(); }
  const std::string& get_tileselect_object() const { return tileselect.m_object; }

  EditorInputGui::InputType get_tileselect_input_type() const { return tileselect.m_input_type; }

  int get_tileselect_select_mode() const;
  int get_tileselect_move_mode() const;

  std::string get_levelfile() const { return levelfile; }

  void set_level(const std::string& levelfile_) {
    levelfile = levelfile_;
    reload_request = true;
  }

  std::string get_level_directory() const;

  void open_level_directory();

  void set_worldmap_mode(bool new_mode) { worldmap_mode = new_mode; }
  bool get_worldmap_mode() const { return worldmap_mode; }

  bool is_testing_level() const { return leveltested; }

  /** Checks whether the level can be saved and does not contain
      obvious issues (currently: check if main sector and a spawn point
      named "main" is present) */
  void check_save_prerequisites(bool& sector_valid, bool& spawnpoint_valid) const;

  void load_sector(const std::string& name);
  void load_sector(size_t id);

  void update_node_iterators();
  void esc_press();
  void delete_markers();
  void sort_layers();

  void select_tilegroup(int id);
  const std::vector<Tilegroup>& get_tilegroups() const;
  void change_tileset();

  void select_objectgroup(int id);
  const std::vector<ObjectGroup>& get_objectgroups() const;

  // speed is in tiles per frame
  void scroll_up(float speed = 1.0f);
  void scroll_down(float speed = 1.0f);
  void scroll_left(float speed = 1.0f);
  void scroll_right(float speed = 1.0f);

  bool is_level_loaded() const { return levelloaded; }

  void edit_path(Path* path, GameObject* new_marked_object) {
    inputcenter.edit_path(path, new_marked_object);
  }

  void add_layer(GameObject* layer) {
    layerselect.add_layer(layer);
  }

  GameObject* get_selected_tilemap() const { return layerselect.m_selected_tilemap; }

private:
  void reload_level();
  void load_layers();
  void quit_editor();
  void save_level();
  void test_level();
  void update_keyboard(const Controller& controller);

  bool can_scroll_horz() const;
  bool can_scroll_vert() const;

protected:
  std::unique_ptr<Level> level;
  std::unique_ptr<World> world;

  std::string levelfile;
  std::string test_levelfile;
  bool worldmap_mode;

public:
  bool quit_request;
  bool newlevel_request;
  bool reload_request;
  bool reactivate_request;
  bool deactivate_request;
  bool save_request;
  bool test_request;

  std::unique_ptr<Savegame> m_savegame;

  Sector* currentsector;

private:
  bool levelloaded;
  bool leveltested;

  TileSet* tileset;

  EditorInputCenter inputcenter;
  EditorInputGui tileselect;
  EditorLayersGui layerselect;
  EditorScroller scroller;

  bool enabled;
  SurfacePtr bgr_surface;

private:
  Editor(const Editor&) = delete;
  Editor& operator=(const Editor&) = delete;
};

#endif

/* EOF */
