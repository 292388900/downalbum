

function GetBaiduBlogAccount(url)
	local pos1=nil
	local pos2=nil
	local name=nil

	--�ҰٶȲ��͵�����
	pos2,pos1 = string.find(url,'hi.baidu.com/')
	if pos1==nil then
		return nil
	end 

	--���ϴβ��ҵĽ���λ�ÿ�ʼ��/
	pos1 = pos1+1
	pos2,_ = string.find(url,'/',pos1)
	if pos2==nil then
		pos2 = string.len(url)
	else 
		pos2 = pos2-1
	end 

	return string.sub(url,pos1,pos2)
end


--����
local s1 = 'http://hi.baidu.com/469356814/blog'
local s2 = 'http://hi.baidu.com/469356814'
local s3 = 'http://hi.baidu.com'

name = GetBaiduBlogAccount(s1)
print(name)
name = GetBaiduBlogAccount(s2)
print(name)
name = GetBaiduBlogAccount(s3)
print(name)