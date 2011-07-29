
--通过百度博客网址获取百度博客账号
function GetBaiduBlogAccount(url)
	local pos1=nil
	local pos2=nil
	local name=nil

	--找百度博客的域名
	pos2,pos1 = string.find(url,'hi.baidu.com/')
	if pos1==nil then
		return nil
	end 

	--从上次查找的结束位置开始找/
	pos1 = pos1+1
	pos2,_ = string.find(url,'/',pos1)
	if pos2==nil then
		pos2 = string.len(url)
	else 
		pos2 = pos2-1
	end 

	return string.sub(url,pos1,pos2)
end



--通过百度博客首页源码获取百度博客昵称
function GetBaiduBlogName(s)
  -- Remove optional spaces from the tags.
  s = string.gsub(s, "\n", " ")
  s = string.gsub(s, " *< *", "<")
  s = string.gsub(s, " *> *", ">")

  -- Put all the tags in lowercase.
  s = string.gsub(s, "(<[^ >]+)", string.lower)

  local i, f, t = string.find(s, "<title>(.+)_百度空间")
  return t or ""
end


--测试
local s1 = 'http://hi.baidu.com/469356814/blog'
local s2 = 'http://hi.baidu.com/469356814'
local s3 = 'http://hi.baidu.com'

name = GetBaiduBlogAccount(s3)
print(name)
name = GetBaiduBlogAccount(s1)
print(name)
name = GetBaiduBlogAccount(s2)
print(name)

homepage = 'http://hi.baidu.com/' .. name

--[[此处获取源码略,这里以这个首页源码为例:
http://hi.baidu.com/%B0%AE%B9%FD%EC%E1%C9%CB%D0%C4%C1%CB'
]]

homepage = 'http://hi.baidu.com/%B0%AE%B9%FD%EC%E1%C9%CB%D0%C4%C1%CB'

require 'socket'
local http = require 'socket.http'
htmlsrc = assert(http.request(homepage))
name = GetBaiduBlogName(htmlsrc)
print(name)