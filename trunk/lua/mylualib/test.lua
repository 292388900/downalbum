--[[TESTCODE]]

require "mylib"

dir , path, name = mylib.getcurluapath()

local imgpath = dir .. 'img\\'
local imgfile = nil
local url = nil
local index = 1
local pos1 = nil
local pos2 = nil
local html = mylib.gethtml('http://xiu.mop.com/artical/11070800113226.shtml')
html = mylib.utf8togbk(html)

local start = string.find(html,[[class="content"]])
local over = string.find(html,[[class="ckyt"]])

while true do
	_, pos1 = string.find(html,[[src="]],start)
	if pos1 == nil or pos1 > over then 
		break
	end

	pos2 = string.find(html,[["]],pos1+1)

	url = string.sub(html,pos1+1,pos2-1)
	--print(url)
	
	imgfile = imgpath .. index .. '.jpg'
	mylib.downloadfile(url,imgfile)
	
	start = pos2
	index = index + 1
end 

mylib.msgbox('over')


--mylib.msgbox( mylib.downloadfile('http://www.baidu.com',dir .. '1.html') )
--mylib.msgbox(dir,path ,name)
--mylib.msgbox(mylib.getcurluapath())

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

