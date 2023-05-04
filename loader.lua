-- Loader to find and load .lua.gz files
local gzlua = require("gzlua")
table.insert(package.loaders, 2, function(module)
    module = string.gsub(module, "%.", "/")
    local error = ""
    for path in string.gmatch(package.path, "([^;]+)") do
        local filename = string.gsub(path, "%?", module) .. ".gz"
        local file_content = gzlua.load(filename)
        if file_content then
            return loadstring(file_content, filename)
        end
        error = error .. "\n\tno file '" .. filename .. "'"
    end
    return error
end)