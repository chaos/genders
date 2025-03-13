#!/usr/bin/lua

db_file = "testgenders"

genders_lib = require("genders")
genders_handle = nil

-- wrapper to for using pcall
function wrapper_new() 
  genders_handle = genders_lib.new(db_file)
end

str_err = ""
str_out = ""

no_load = nil
error_status = 0

getter_funcs = {"getnumattrs","getnumnodes","getnodes"}
list_nodes = {"foobar","hype355","null"}
list_attr = {"foobar","mgmt"}
-- attr <-> value mismatch as this is dict!
list_attrval = {["foobaar"] = "cfhost", ["hypei"] = "cfhost"}
list_query = { "mgmt","mgmt||login","mhmt&&login","~mgmt"}

local open_lib, genders_handle = pcall(genders_lib.new,db_file,genders_lib)
if open_lib then
  print("Loaded database \""..db_file.."\"")
  for _, func_name in ipairs(getter_funcs) do 
    local str = "return function(handle) return pcall(handle."..func_name..",handle) end"
    local wrap_func = assert(load(str))
    local func = wrap_func()
    local ok,ret_val = func(genders_handle) 
    if ok then 
      if type(ret_val) ~= "table" then 
        print(func_name.." :: "..ret_val)
      else
        local tmp_str = func_name.." :: "
        for _,val in ipairs(ret_val) do
          tmp_str = tmp_str..val..", "
        end
        print(tmp_str)
      end
    else
      error_status = 1
      str_err = "Could not call "..func_name..": "..ret_val
    end
  end
  local ok, ret_val = pcall(genders_handle.getnodes,genders_handle,"mgmt")
  if ok then 
      local tmp_str = "getnodes(\"mgmt\") :: " for _,val in ipairs(ret_val) do tmp_str = tmp_str..val..", " end
      print(tmp_str)
  else
    error_status = 1
    str_err = "Could not call: getnodes(\"mgmt\")"..ret_val
  end
  local ok, ret_val = pcall(genders_handle.getnodes,genders_handle,"foobarlalala")
  if ok then 
      local tmp_str = "getnodes(\"foobarlalala\") :: " for _,val in ipairs(ret_val) do tmp_str = tmp_str..val..", " end
      print(tmp_str)
  else
    error_status = 1
    str_err = "Could not call: getnodes(\"foobarlalala\")"..ret_val
  end
  local ok, ret_val = pcall(genders_handle.getnodes,genders_handle,"cfhost","foobar")
  if ok then 
      local tmp_str = "getnodes(\"cfhost\",\"foobar\") :: " for _,val in ipairs(ret_val) do tmp_str = tmp_str..val..", " end
      print(tmp_str)
  else
    error_status = 1
    str_err = "Could not call: getnodes(\"cfhost\",\"foobar\")"..ret_val
  end
  local ok, ret_val = pcall(genders_handle.getnodes,genders_handle,"cfhost","hypei")
  if ok then 
      local tmp_str = "getnodes(\"cfhost\",\"hypei\") :: " for _,val in ipairs(ret_val) do tmp_str = tmp_str..val..", " end
      print(tmp_str)
  else
    error_status = 1
    str_err = "Could not call: getnodes(\"cfhost\",\"hypei\")"..ret_val
  end
  local ok, ret_val = pcall(genders_handle.getattr,genders_handle,"hype355")
  if ok then 
      local tmp_str = "getattr(\"hype355\") :: " for key,value in pairs(ret_val) do tmp_str = tmp_str..key if value == "" then tmp_str = tmp_str..", " else tmp_str = tmp_str.."="..value.."," end  end
      print(tmp_str)
  else
    error_status = 1
    str_err = "Could not call: getattr(\"hype355\")"..ret_val
  end
  for _,node in ipairs(list_nodes) do
    local ok, ret_val = pcall(genders_handle.isnode,genders_handle,node)
    if ok then
      if ret_val then print("isnode("..node..") = true") else print("isnode("..node..") = false") end
    else
      error_status = 1
      str_err = "Could not call: isnode("..node..") "..ret_val
    end
  end
  for _,attr in ipairs(list_attr) do
    local ok, ret_val = pcall(genders_handle.isattr,genders_handle,attr)
    if ok then
      if ret_val then print("isattr("..attr..") = true") else print("isattr("..attr..") = false") end
    else
      error_status = 1
      str_err = "Could not call: isattr("..attr..") "..ret_val
    end
  end
  for value,attr in pairs(list_attrval) do
    local ok, ret_val = pcall(genders_handle.isattrval,genders_handle,attr,value)
    if ok then
      if ret_val then print("isattrval("..attr..","..value..") = true") else print("isattrval("..attr..","..value..") = false") end
    else
      error_status = 1
      str_err = "Could not call: isattr("..attr..") "..ret_val
    end
  end
  for _,qry in ipairs(list_query) do
    local ok, ret_val = pcall(genders_handle.query,genders_handle,qry)
    if ok then
      local tmp_str = "query("..qry..") :: " for _,val in pairs(ret_val) do tmp_str = tmp_str..val..", " end
      print(tmp_str)
    else
      error_status = 1
      str_err = "Could not call: query("..qry..")"..ret_val
    end
  end
else
  str_err = "Failure in loading database \""..db_file.."\""
  error_status = 1
end
if error_status == 0 then
  print("No errors")
else
  print("Collected errors are: "..str_err)
end
