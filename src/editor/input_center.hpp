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

#ifndef HEADER_SUPERTUX_EDITOR_INPUT_CENTER_HPP
#define HEADER_SUPERTUX_EDITOR_INPUT_CENTER_HPP

#include "control/input_manager.hpp"
#include "math/vector.hpp"

class Color;
class DrawingContext;
class Editor;
class GameObject;
class MovingObject;
class NodeMarker;
class Path;
class Rectf;
class Tip;

class EditorInputCenter final
{
public:
  static bool render_background;
  static bool render_grid;
  static bool snap_to_grid;
  static int selected_snap_grid_size;

public:
  EditorInputCenter(Editor& editor);

  void event(SDL_Event& ev);
  void draw(DrawingContext&);
  void update(float dt_sec);

  void update_pos();
  void delete_markers();
  void update_node_iterators();

  void edit_path(Path* path, GameObject* new_marked_object = nullptr);

private:
  void input_tile(const Vector& pos, uint32_t tile);
  void put_tile();
  void draw_rectangle();
  void fill();
  void put_object();

  void rubber_object();
  void rubber_rect();

  void grab_object();
  void move_object();
  void clone_object();
  void hover_object();
  void set_object();
  void mark_object();
  void add_path_node();

  void draw_tile_tip(DrawingContext&);
  void draw_tile_grid(DrawingContext&, const Color& line_color, int tile_size = 32);
  void draw_tilemap_border(DrawingContext&);
  void draw_path(DrawingContext&);

  void process_left_click();
  void process_right_click();

  // sp is sector pos, tp is pos on tilemap.
  Vector tp_to_sp(const Vector& tp, int tile_size = 32);
  Vector sp_to_tp(const Vector& sp, int tile_size = 32);
  Vector tile_screen_pos(const Vector& tp, int tile_size = 32);

  // in sector position
  Rectf drag_rect();
  Rectf tile_drag_rect();
  Rectf selection_draw_rect();
  void update_tile_selection();

private:
  Editor& m_editor;
  Vector m_hovered_tile;
  Vector m_sector_pos;
  Vector m_mouse_pos;

  bool m_dragging;
  bool m_dragging_right;
  Vector m_drag_start;
  MovingObject* m_dragged_object;
  MovingObject* m_hovered_object;
  GameObject* m_marked_object;
  Path* m_edited_path;
  NodeMarker* m_last_node_marker;
  std::unique_ptr<Tip> m_object_tip;
  Vector m_obj_mouse_desync;

private:
  EditorInputCenter(const EditorInputCenter&) = delete;
  EditorInputCenter& operator=(const EditorInputCenter&) = delete;
};

#endif

/* EOF */
