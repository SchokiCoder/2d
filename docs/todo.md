# movement

- add crouching
- add crouch slide

# renaissance

- before actually implementing new features, make a goals.md

## image to world conversion

Each pixel is a block.
1)   Draw image                        PNG
2.1) Assign color codes to block id's  JSON
2.2) Set entities                      JSON
3)   Compile                           Compiler "Oven"

# old problems

+ add fsanitize to compile
+ replace almost all "chemarium" strings with "generic2d"
+ relicense under gpl v2 or later
+ repo rename generic2d
+ replace remnants of stdbool with int
  Where supposed to be removed long ago.
- fix all mem issues
	- fix heap-buffer-overflow "String_new engine/sstring.c:44"
	  just yoink hui/hstring?
- consider zig cc
- unused variable warnings
  (most likely because of -c flag oriented compilation)
- motd is bugged
- set version to 0.1.0

-----

# canceled
- add image fontmap
  monospcae, utf-8, no rgb image
  represent ALL characters as filled square except:
  latin characters, numbers, common special chars ,;.:-_ etc
  (by including all utf-8 characters we can iterate
  through the parsed data with the actual character like 'a')

- remove block-based world building
  add images as maps
  In this case, i think a hotswap system for quickly changing
  maps is needed. So that developing a map as a picture can be
  easily cut into multiple pictures.
  Maybe a trigger could be named via a layer name of a XCF-file.
  Those layers could then be given behavior with a LUA script.
  A layer could also just be a static thing and in the extending
  LUA file properties could be given such as "is a liquid" or
  "not solid".
  In a lua file you could assign the existing layernames an
  actual purpose.
  Or maybe JSON-file?
 
  removing block based maps is not going work
  (without replacing by something very complex)

# rehab

- finish Makefile
- fix application metadata
- fix linker error
- Config_save -> Config_to_file
- are debug builds actually valid
  (because o-files are not compiled with -g or -D _DEBUG)
- same ^ for -Os
- fix segfault
  this is due to not finding the font
  (has no NULL check lol)
  actual fix postponed due to obsolesence
- double free on game start
- add cc sa license to assets


# refactor

- remove typedef from struct
  GNU indent messed my pointers up: "muh * ptr"

## bool -> int

- engine
- gui
- main
