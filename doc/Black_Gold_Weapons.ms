if (heapSize < 200000) then
        heapSize = 2000000 -- allow ~ 40 MB instead of just 7.5 MB. Prevents "Runtime Error: Out of scripter memory"	
fname = getOpenFileName \
caption:"Open .* from Mesh folder" \
types:"Black Gold Meshes (*.xmod)|*.xmod" \
historyCategory:"BlackGoldMeshObjectPresets"
f = fopen fname "rb"

clearlistener()
fn readFixedString bstream fixedLen = (
local str = ""
for i = 1 to fixedLen do (
str += bit.intAsChar (ReadByte bstream #unsigned))
str
)
struct weight_data
(
	boneids,weights
)
clearlistener()

MHeader = readFixedString f 4	
Version = readlong f
Unk1 = readlong f
NumObj =readlong f	
FnameSize = readlong f
Fname = readFixedString f FnameSize
Ctrl = readlong f
FPS = readlong f

fseek f 0x1C #seek_cur
	
Unk2 = readlong f
	
for i=1 to NumObj do (
	
	
ModelNameSize = readlong f
ModelName = readFixedString f ModelNameSize
Num2 = readlong f
Type1 = readshort f	
Type2 = readshort f		
	

if Type2==2 then fseek f 0x44 #seek_cur	
if Type2==0 then (
fseek f 0x2C #seek_cur	
ModelNameSize2 = readlong f
ModelName2 = readFixedString f ModelNameSize2
Num02 = readlong f
Type01 = readshort f	
Type02 = readshort f	
fseek f 0x44 #seek_cur		
)	

Print ("Model Start @ 0x"+((bit.intAsHex(ftell f))as string))

SubCount = readlong f	
		
vertArray = #()
Facearray = #()	
UV_array = #()
		
MNameSize = readlong f
MName = readFixedString f MNameSize	

ModelId = readlong f
Unk4 = readlong f

VertexCount = readlong f

Print ("Vertex Start @ 0x"+((bit.intAsHex(ftell f))as string))
	
if Type1==4101 then 
for x=1 to VertexCount do (	

vx = Readfloat f
vy = Readfloat f
vz = Readfloat f
	
nx = Readfloat f
ny = Readfloat f
nz	= Readfloat f

tu= Readfloat f	
tv= Readfloat f*-1
			   
append vertArray ([vx,vz,vy]*100)
append UV_array [tu,tv,0]	 
)	
	
if Type1==5381 then 
	
for x = 1 to VertexCount do (
	
getPos = ftell f + 56		

vx = Readfloat f
vy = Readfloat f
vz = Readfloat f

nx = Readfloat f
ny = Readfloat f
nz	= Readfloat f
	
bix = Readfloat f
biy = Readfloat f
biz = Readfloat f	
	
tx = Readfloat f
ty = Readfloat f
tz	= Readfloat f
	
tu= Readfloat f	
tv= Readfloat f*-1	

unk=readfloat f	
	
fseek f getPos #seek_set 	   
append vertArray ([vx,vz,vy]*100)
append UV_array [tu,tv,0]	 
)	

if Type1==5397 then 
	
for x = 1 to VertexCount do (
	
getPos = ftell f + 60		

vx = Readfloat f
vy = Readfloat f
vz = Readfloat f

nx = Readfloat f
ny = Readfloat f
nz	= Readfloat f

bix = Readfloat f
biy = Readfloat f
biz = Readfloat f	
	
tx = Readfloat f
ty = Readfloat f
tz	= Readfloat f
	
tu= Readfloat f	
tv= Readfloat f*-1	
	
fseek f getPos #seek_set 	   
append vertArray ([vx,vz,vy]*100)
append UV_array [tu,tv,0]	 
)	

FaceCount = readlong f

print FaceCount

Print ("Face Start @ 0x"+((bit.intAsHex(ftell f))as string))

for x = 1 to FaceCount/3 do (
f1 = (Readshort f #unsigned + 1)
f2 = (Readshort f #unsigned + 1)
f3 = (Readshort f #unsigned + 1)
append Facearray [f3,f2,f1]
)

Print ("Face End @ 0x"+((bit.intAsHex(ftell f))as string))

try(
msh = mesh vertices:vertArray faces:faceArray
msh.name=ModelName
msh.numTVerts = UV_array.count
buildTVFaces msh
for j = 1 to UV_array.count do setTVert msh j UV_array[j]
for j = 1 to faceArray.count do setTVFace msh j faceArray[j]
msh.displayByLayer = false
msh.backfacecull = true
msh.wirecolor = (color 230 200 210)

)catch(format "Error!!!\n")

Print ("Mesh End@ 0x"+((bit.intAsHex(ftell f))as string))
)
	
fclose f	