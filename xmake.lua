set_project("ivp")
set_version("1.0.0")

set_languages("cxx17")

set_targetdir("lib")

local function is_directory_empty(directory)
    local files = os.files(path.join(directory, "*"))
    return #files == 0
end

local is_building_all = false

target("ivp")
    set_kind("static") 
    add_includedirs("include")
    if not is_directory_empty("src") then
        add_files("src/*.cpp")
    else
        print("src no file")
    end

target("test")
    set_kind("binary")
    set_targetdir("bin")
    add_includedirs("include")
    add_deps("ivp")
    if not is_directory_empty("test") then
        add_files("test/*.cpp")
    else
        print("test no file")
    end


target("report")
    set_kind("phony")  -- 标记为伪目标，不会产生实际的二进制文件

    -- 添加要编译的文件
    add_files("report/dissertation/report.tex")
    before_build(function(target)
    end)

    -- 定义编译规则
    on_build(function (target)
        if is_building_all then
            os.exec("xmake run test")
            os.exec("xmake run plot")
        end
    end)

    -- 定义编译规则
    on_build(function (target)
        import("core.base.option")
        import("core.base.global")
        import("core.project.task")
        import("core.tool.toolchain")
        import("core.base.scheduler")

        local latex = "xelatex"
        local bibtex = "bibtex"
        local report_dir = path.join(os.projectdir(), "report", "dissertation")
        local report_tex = "report.tex"
        local output_pdf = "report.pdf"
        local destination_dir = path.join(os.projectdir(), "report")  -- 指定目标目录

        -- 切换到正确的工作目录并执行命令
        scheduler.co_start(function ()
            os.cd(report_dir)
            
            -- 定义要执行的命令，添加 -shell-escape 标志
            local commands = {
                -- {latex, "-shell-escape", report_tex},
                -- {bibtex, "report"},
                {latex, "-shell-escape", report_tex},
                {latex, "-shell-escape", report_tex}
            }

            -- 执行命令
            for _, cmd in ipairs(commands) do
                os.vrunv(cmd[1], {cmd[2], cmd[3]})
            end

            -- 将生成的 PDF 文件移动到指定目录
            os.cp(output_pdf, destination_dir)

            -- 删除辅助文件
            local aux_files = {"*.aux", "*.bbl", "*.blg", "*.log", "*.out", "*.toc", "*.lof", "*.lot"}
            for _, pattern in ipairs(aux_files) do
                os.tryrm(pattern)
            end
        end)
    end)


target("plot")
    set_kind("phony")
    on_run(function ()
        os.exec("python plot/plot.py")
    end)

target("all")
    set_kind("phony")
    add_deps("ivp", "test", "report")
    before_build(function(target)
        is_building_all = true
    end)
    after_build(function(target)
        is_building_all = false
    end)

target("clean")
    set_kind("phony")
    on_clean(function ()
        os.rm("lib")
        os.rm("bin")
        os.rm("build")
        os.rm("report/report.pdf")
        os.rm("img")
        os.rm("output")
        os.rm("log")
    end)

if is_host("windows") then
    add_cxflags("-D_CRT_SECURE_NO_WARNINGS")
elseif is_host("linux") then
    add_cxflags("-fPIC")
end



