//  SuperTux
//  Copyright (C) 2018 Tobias Markus <tobbi@supertux.org>
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

#include "supertux/menu/editor_layers_menu.hpp"

#include <boost/format.hpp>
#include <limits.h>
#include <sstream>

#include "audio/sound_manager.hpp"
#include "editor/editor.hpp"
#include "editor/layer_icon.hpp"
#include "editor/object_menu.hpp"
#include "gui/dialog.hpp"
#include "gui/menu_item.hpp"
#include "supertux/game_manager.hpp"
#include "supertux/globals.hpp"
#include "supertux/level.hpp"
#include "supertux/menu/menu_storage.hpp"
#include "supertux/screen_fade.hpp"
#include "supertux/screen_manager.hpp"
#include "supertux/tile_set.hpp"
#include "supertux/title_screen.hpp"
#include "supertux/world.hpp"
#include "util/file_system.hpp"
#include "util/gettext.hpp"

EditorLayersMenu::EditorLayersMenu() :
  selected_item_idx()
{
  refresh();
}

EditorLayersMenu::~EditorLayersMenu()
{
  auto editor = Editor::current();
  if(editor == NULL) {
    return;
  }
  editor->reactivate_request = true;
}

void
EditorLayersMenu::refresh()
{
  auto it = Editor::current()->layerselect.layers.begin();
  while(it != Editor::current()->layerselect.layers.end())
  {
    auto layer = (*it).get();
    if(!layer->is_valid())
      it = Editor::current()->layerselect.layers.erase(it);
    else
      ++it;
  }
  clear();
  add_label(_("Layers"));
  add_hl();

  int id = 0;
  for(auto& layer : Editor::current()->layerselect.layers) {
    if(!layer->is_valid())
    {
      continue;
    }

    auto layer_name = layer->layer->get_name();
    if(layer_name.empty())
    {
      layer_name = layer->layer->get_display_name();
    }
    auto z_pos = layer->get_zpos();
    if(z_pos > INT_MIN)
    {
      add_icon_entry(id, str(boost::format(_("%s (%s)")) % layer_name % z_pos), layer->surface);
    }
    else
    {
      add_icon_entry(id, str(boost::format(_("%s")) % layer_name), layer->surface);
    }

    auto tilemap = dynamic_cast<TileMap*>(layer->layer);
    if(tilemap && tilemap->editor_active)
    {
      selected_item_idx = id;
    }

    id++;
  }

  add_hl();
  add_entry(-1, _("Remove"));
  add_entry(-2, _("Add new layer"));
  add_entry(-3, _("Layer options"));

  add_hl();
  add_back(_("Back"));
}
void
EditorLayersMenu::draw_item(DrawingContext& context, int index)
{
  Menu::draw_item(context, index);
  auto pos = get_center_pos();
  auto menu_width_ = get_width();
  float y_pos = pos.y + 24 * index - get_height() / 2 + 12;

  // Because the title and the horizontal line count as items on their own
  // let's add 2 to the selected index:
  if(index == selected_item_idx + 2)
  {
    context.draw_filled_rect(Rectf(Vector(pos.x - menu_width_/2 + 10, y_pos - 12),
                                   Vector(pos.x + menu_width_/2 - 10, y_pos + 12)),
                             Color(1.0f, 0, 0, 0.4f),
                             12.0f,
                             LAYER_GUI-10);
  }
}

TileMap*
EditorLayersMenu::get_next_tilemap(const GameObject* object) const
{
  auto editor = Editor::current();
  auto& layers = editor->layerselect.layers;
  auto obj_as_tilemap = dynamic_cast<const TileMap*>(object);
  TileMap *previous = NULL, *next = NULL;
  bool found = false, next_found = false;

  for(const auto& layer : layers)
  {
    if(dynamic_cast<TileMap*>(layer->layer))
    {
      if(layer->layer == obj_as_tilemap)
      {
        found = true;
      }
      else
      {
        next_found = true;
        if(!found)
        {
          previous = (TileMap*)(layer->layer);
        }
        else if(found && next == NULL)
        {
          next = (TileMap*)(layer->layer);
        }
      }
    }
  }
  if(previous != NULL)
  {
    return previous;
  }
  
  if(next != NULL)
  {
    return next;
  }
  return NULL;
}

void
EditorLayersMenu::menu_action(MenuItem* item)
{
  auto editor = Editor::current();
  auto& layers = editor->layerselect.layers;
  auto& selected_tilemap = editor->layerselect.selected_tilemap;
  if (item->id >= 0)
  {
    auto& new_layer = editor->layerselect.layers[item->id]->layer;
    
    if(dynamic_cast<TileMap*>(new_layer) != NULL)
    {
      selected_tilemap->editor_active = false;
      selected_tilemap = dynamic_cast<TileMap*>(new_layer);
      selected_tilemap->editor_active = true;
    }
    selected_item_idx = item->id;
  }
  if(item->id == -1)
  {
    auto& object = layers[selected_item_idx]->layer;
    if(object == NULL)
    {
      return;
    }
    auto obj_as_tilemap = dynamic_cast<TileMap*>(object);
    if( obj_as_tilemap && selected_tilemap == obj_as_tilemap)
    {
      auto next_tilemap = get_next_tilemap(object);
      if(next_tilemap == NULL)
      {
        std::unique_ptr<Dialog> dialog(new Dialog);
        dialog->set_text(_("There must be at least one tilemap in each sector."));
        dialog->clear_buttons();
        dialog->add_button(_("OK"), [] {});
        MenuManager::instance().set_dialog(std::move(dialog));
        return;
      }
      else
      {
        selected_tilemap = next_tilemap;
        selected_tilemap->editor_active = true;
      }
    }
    object->remove_me();
    object = NULL;

    editor->delete_markers();
    editor->reactivate_request = true;

    refresh();
    return;
  }
  if(item->id == -2)
  {
    MenuManager::instance().push_menu(MenuStorage::EDITOR_NEW_LAYER_MENU);
  }
  if(item->id == -3)
  {
    std::unique_ptr<Menu> om(new ObjectMenu(layers[selected_item_idx]->layer));
    MenuManager::instance().push_menu(move(om));
  }
}

/* EOF */