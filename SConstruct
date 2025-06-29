#!/usr/bin/env python
import os
import sys

from methods import print_error


def normalize_path(val, env):
    return val if os.path.isabs(val) else os.path.join(env.Dir("#").abspath, val)


def validate_parent_dir(key, val, env):
    if not os.path.isdir(normalize_path(os.path.dirname(val), env)):
        raise UserError("'%s' is not a directory: %s" % (key, os.path.dirname(val)))


libname = "Universal_2D_Pathfinder"
projectdir = "Universal_2D_Pathfinder"
# projectdir = "demo"

localEnv = Environment(tools=["default"], PLATFORM="")

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Add(
    BoolVariable(
        key="compiledb",
        help="Generate compilation DB (`compile_commands.json`) for external tools",
        default=localEnv.get("compiledb", False),
    )
)
opts.Add(
    PathVariable(
        key="compiledb_file",
        help="Path to a custom `compile_commands.json` file",
        default=localEnv.get("compiledb_file", "compile_commands.json"),
        validator=validate_parent_dir,
    )
)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()
env["compiledb"] = False

env.Tool("compilation_db")
compilation_db = env.CompilationDatabase(
    normalize_path(localEnv["compiledb_file"], localEnv)
)
env.Alias("compiledb", compilation_db)

submodule_initialized = False
dir_name = 'godot-cpp'
if os.path.isdir(dir_name):
    if os.listdir(dir_name):
        submodule_initialized = True

if not submodule_initialized:
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})
# C:/GITHUB/GitHub_Main_Folder/Godot_Universal_Pathfinding/godot-cpp/SConstruct
# godot-cpp/SConstruct
env.Append(CPPPATH=["src/"])

# debug = ARGUMENTS.get('debug', 0)
# if int(debug):
#     env.Append(CCFLAGS = ['/DEBUG'])



sources = Glob("src/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])
filepath = ""

if env["platform"] == "macos" or env["platform"] == "ios":
    filepath = "framework/"
    file = "{}.{}.{}".format(libname, env["platform"], env["target"])

libraryfile = "demo/bin/{}{}".format(filepath, file)
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

# if env["platform"] == "macos":
#     library = env.SharedLibrary(
#         "demo/bin/libgdexample.{}.{}.framework/libgdexample.{}.{}".format(
#             env["platform"], env["target"], env["platform"], env["target"]
#         ),
#         source=sources,
#     )
# elif env["platform"] == "ios":
#     if env["ios_simulator"]:
#         library = env.StaticLibrary(
#             "demo/bin/libgdexample.{}.{}.simulator.a".format(env["platform"], env["target"]),
#             source=sources,
#         )
#     else:
#         library = env.StaticLibrary(
#             "demo/bin/libgdexample.{}.{}.a".format(env["platform"], env["target"]),
#             source=sources,
#         )
# else:
#     library = env.SharedLibrary(
#         "demo/bin/libgdexample{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
#         source=sources,
#     )

copy = env.InstallAs("{}/bin/{}/{}{}".format(projectdir, env["platform"], filepath, file), library)
# copy = env.InstallAs("{}/bin/{}An_{}".format(projectdir, filepath, file), library)

default_args = [library, copy]
if localEnv.get("compiledb", False):
    default_args += [compilation_db]
Default(*default_args)


if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")