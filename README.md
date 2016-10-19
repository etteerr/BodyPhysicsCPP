# BodyPhysicsCPP

## commandline usage:
GalaxySim1-multi [commands]
  - --script [file] => runs script before creating a renderwindow
  - --dt [dt] => specifies delta T in seconds
  - --sol => start simulation on load
  - --speed [rtf] => sets the speed relative to realtime (rtf=2 == 2x normal time)
  - --help => prints this
  
 ## script language
 The script language is based on single commands and has no syntax support
 No white lines may be present and no comments exist
 
- script [file] => runs script, returns if done
- addrng [amount] [scale] [startx] [starty] => adds randomly generated particles. (random values cannot be changed atm)
- setview [x] [y] => sets view window to x,y
- wait for [time] => waits for time seconds in simulation time has passed
- wait until [time] => waits until time > [simulation time]
- quit => exit(0)
- addlog [index] => adds a logger to object index (in add order). Renders a trace in the view
- add => starts the add particle wizard
- add [opt] => omitted options are set to 0
 - weight mass => sets mass to mass
 - speed spd => sets the speed vector to size spd
 - direction x y => sets the direction of the vector to x,y
 - position x y => sets the position to x,y
 - radius r => sets radius to r
- list => lists all particles
- pause => halts simulation
- resume => resumes simulation
- setSpeed rtf => same as --speed, sets the speed relative to realtime (rtf=2 == 2x normal time)
- unselect => deselects anyparticle
- select x => selects particle x

  The following can be used after select, some can be used without select
  - edit => used the same as add
  - vars => same as list, only lists selected
  - vars [id] => can be used without select
  
