/**
 * This script gets loaded into the squirrel root vm in supertux. So functions
 * and variables you define here can be used in all threads
 * Also, this is bad and cruddy and takes up space, so I minified it
 * Hope that works! :)
 */

//Create Level table
Level <- {
  finish=Level_finish,
  spawn=Level_spawn,
  flip_vertically=Level_flip_vertically,
  toggle_pause=Level_toggle_pause,
  edit=Level_edit
};


function end_level(){play_music("music/misc/leveldone.ogg"),Tux.deactivate(),wait(6),Effect.fade_out(2),wait(2),Level.finish(!0)}function exit_worldmap(){wait_for_screenswitch(),exit_screen()}function levelflip(){Effect.fade_out(1),wait(1),Level.flip_vertically(),Effect.fade_in(1)}function println(e){print(e),print("\n")}Level,finish=Level_finish,spawn=Level_spawn,flip_vertically=Level_flip_vertically,toggle_pause=Level_toggle_pause,edit=Level_edit,"state"in this||state;

