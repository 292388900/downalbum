--[[TESTCODE]]

assert(package.loadlib("c:/mylib.dll", "luaopen_mylib"))()

a=1

t={}

f=function()  end

--
--mylib.msgbox( mylib.gethtml('www.baidu.com') )
--mylib.msgbox( mylib.gethtml('http://www.baidu.com') )

s=mylib.gethtml('http://space.feixin.10086.cn/')
--mylib.msgbox( s )
s=mylib.utf8togbk(s)
mylib.msgbox( s )
--mylib.msgbox( 'a=' , a, 'b=' , b, t, f )

