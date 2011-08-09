--[[TESTCODE]]

function currDir()
  os.execute("cd > cd.tmp")
  local f = io.open("cd.tmp", r)
  local cwd = f:read("*a")
  f:close()
  os.remove("cd.tmp")
  return cwd
end

dir= currDir()
dir =  string.sub(dir,1,-2) .. '\\'	--最后有一个换行符

assert(package.loadlib(dir .. "debug\\mylib.dll", "luaopen_mylib"))()

a=1

t={}

f=function()  end

dir , path, name = mylib.getcurluapath()
mylib.msgbox(dir,path ,name)
mylib.msgbox(mylib.getcurluapath())

--dir = mylib.getcurluapath()
--mylib.msgbox(dir)

--
--mylib.msgbox( mylib.gethtml('www.baidu.com') )
--mylib.msgbox( mylib.gethtml('http://www.baidu.com') )

--s=mylib.gethtml('http://space.feixin.10086.cn/')
--mylib.msgbox( s )
--s=mylib.utf8togbk(s)
--mylib.msgbox( s )
--mylib.msgbox( 'a=' , a, 'b=' , b, t, f )

