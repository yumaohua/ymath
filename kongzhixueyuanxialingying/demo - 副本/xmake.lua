set_project("demo")

set_languages("c++17")

add_rules("mode.debug", "mode.release")

target("demo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")

    if is_mode("debug") then
        set_optimize("none")
        add_defines("DEBUG")
    else
        set_optimize("fast")
    end

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_includedirs("include")

    if is_mode("debug") then
        set_optimize("none")
        add_defines("DEBUG")
    else
        set_optimize("fast")
    end
