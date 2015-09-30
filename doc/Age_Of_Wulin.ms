if (heapSize < 200000) then
        heapSize = 2000000 -- allow ~ 40 MB instead of just 7.5 MB. Prevents "Runtime Error: Out of scripter memory"	
gname = getOpenFileName \
caption:"Open .* from Bones folder" \
types:"Age Of Wulin Bones (*.xskt)|*.xskt" \
historyCategory:"Age_Of_WulinMeshObjectPresets"
g = fopen gname "rb"
fname = getOpenFileName \
caption:"Open .* from Mesh folder" \
types:"Age Of Wulin Meshes (*.xmod)|*.xmod" \
historyCategory:"Age_Of_WulinMeshObjectPresets"
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

fseek g 0x0 #seek_end
endOfFile = ftell g
	
fseek g 0x0 #seek_set
Header = readFixedString g 4
Ver = readlong g
Unk1 = readlong g
FileNameSize = readlong g
FileName = readFixedString g FileNameSize
Unk2 = readlong g
AnimCount = readlong g
FPS = readfloat g
fseek g 0x18 #seek_cur
BId = readlong g	

BNArr = #()

while ftell g < endOfFile do (

BoneNameSize=readlong g	
BoneName=readFixedString g BoneNameSize
	
BoneType = readlong g
if BoneType==4 then 
(	
AnimType = (readlong g)	 
AnimId = readlong g	 
	  
   px = readFloat g
   py = readFloat g
   pz = readFloat g
   BONEposition = ([px,py,pz]*100)

   rx = readFloat g
   ry = readFloat g
   rz = readFloat g
   rw = readFloat g
   BoneRotation = (quat rx ry rz rw) as matrix3
	   
   sx = readFloat g
   sy = readFloat g
   sz = readfloat g
   BONEscale = [sx,sy,sz]
   
if AnimType==0 then 
for x = 1 to AnimCount do (
rx1 = readshort g
ry1 = readshort g
rz1 = readshort g
)	
if AnimType==1 then 
for x = 1 to AnimCount do (
rx = readshort g
ry = readshort g
rz	= readshort g
a11 = ReadFloat g; a12 = ReadFloat g; a13 = ReadFloat g	
)	
if AnimType==2 then 
for x = 1 to AnimCount do (
rx1 = readshort g
ry1 = readshort g
rz1	= readshort g
a11 = ReadFloat g; a12 = ReadFloat g; a13 = ReadFloat g	
)
if AnimType==8 then 
for x = 1 to 3 do (
rx = readshort g
ry = readshort g
rz	= readshort g
)	
if AnimType==10 then 
for x = 1 to 3 do (
rx = readshort g
ry = readshort g
rz	= readshort g
a11 = ReadFloat g; a12 = ReadFloat g; a13 = ReadFloat g	
)	
)	

if BoneType==3 then 
(	
AnimId = readlong g	 
		  
   px = readFloat g
   py = readFloat g
   pz = readFloat g
   BONEposition = ([px,py,pz]*100)

   rx = readFloat g
   ry = readFloat g
   rz = readFloat g
   rw = readFloat g
   BoneRotation =  (quat rx ry rz rw) as matrix3
	   
   sx = readFloat g
   sy = readFloat g
   sz = readfloat g
   BONEscale = [sx,sy,sz]

if AnimId==6 then fseek g 0x36 #seek_cur		
	
)
	
num_child = readlong g

   bne = dummy showLinks:true showLinksOnly:false
   bne.scale = BONEscale
   bne.name = BoneName
   bne.rotation = BoneRotation
   bne.pos = BONEposition 

if BNArr.count > 0 then (
bne.parent = BNArr[BNArr.count]
bne.transform *= bne.parent.transform
deleteitem BNArr BNArr.count
)
for j = 1 to num_child do append BNArr bne
)
  
rootRT = bone name:"Main_Root" boxsize:[5.0,5.0,5.0]	
boneMain= getnodebyname "Bip01"
if boneMain==undefined then boneMain=getnodebyname "Bone01"
boneMain.parent = rootRT
rootRT.transform = (matrix3 [1,0,0] [0,0,1] [0,1,0] [0,0,0])	

Print ("Bone End @ 0x"+((bit.intAsHex(ftell g))as string))

fclose g	

MHeader = readFixedString f 4	
Version = readlong f
Unk1 = readlong f
NumObj = readlong f
FnameSize = readlong f
Fname = readFixedString f FnameSize
Unk3 = readlong f
Num1 = readlong f

fseek f 0x1C #seek_cur
	
Unk2 =readlong f	
		
for i=1 to NumObj  do (
	
TCount= #()
		
ModelNameSize = readlong f
ModelName = readFixedString f ModelNameSize
Num2 = readlong f
Type1 = readshort f	
Type2 = readshort f		
	
fseek f 0x44 #seek_cur	

SubCount = readlong f	
	
for i=1 to SubCount do (	
			
vertArray = #()
Facearray = #()	
UV_array = #()
Weight_array =#()
BoneMap =#()
BoneNamePosArray =#()	
TexArray =#()		
	
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

WeightsStart = (ftell f)	

Print ("Weights Start@ 0x"+((bit.intAsHex(ftell f))as string))
	
for x = 1 to VertexCount do 
(
getPos = ftell f + 32		
fseek f getPos #seek_set 	 
)	

Print ("BoneMap Start@ 0x"+((bit.intAsHex(ftell f))as string))

BoneMapSize = readlong f
BoneMapStart = ftell f
	
while ftell f<(BoneMapStart+BoneMapSize) do	(
BoneNamePos=ftell f	
BoneMapName = readstring f
append BoneNamePosArray (BoneNamePos as integer)
)	

fseek f BoneMapStart  #seek_set

for i=1 to BoneNamePosArray.count do (
BMapName = readstring f
append BoneMap (BMapName)
)	
		
fseek f WeightsStart #seek_set	
	
for i = 1 to VertexCount do 
(
 			
bone1Off = (readlong f as integer)
B1Offset	=(bone1Off+BoneMapStart)
for i=1 to BoneNamePosArray.count do (
if BoneNamePosArray[i]==B1Offset then B1Offset=i
)
Bone1=(B1Offset as integer)
weight1 = readfloat f	

bone2Off = (readlong f as integer)
B2Offset	=(bone2Off+BoneMapStart)
for i=1 to BoneNamePosArray.count do (
if BoneNamePosArray[i]==B2Offset then B2Offset=i
)
Bone2=(B2Offset as integer)
weight2 = readfloat f
	
bone3Off = (readlong f as integer)
B3Offset	=(bone3Off+BoneMapStart)
for i=1 to BoneNamePosArray.count do (
if BoneNamePosArray[i]==B3Offset then B3Offset=i
)
Bone3=(B3Offset as integer)
weight3 = readfloat f	

bone4Off = (readlong f as integer)
B4Offset	=(bone4Off+BoneMapStart)
for i=1 to BoneNamePosArray.count do (
if BoneNamePosArray[i]==B4Offset then B4Offset=i
)
Bone4=(B4Offset as integer)
weight4 = readfloat f

w = (weight_data boneids:#() weights:#())
maxweight = 0
if(weight1 != 0) then
   maxweight = maxweight + weight1
if(weight2 != 0) then
   maxweight = maxweight + weight2
if(weight3 != 0) then
   maxweight = maxweight + weight3
if(weight4 != 0) then
   maxweight = maxweight + weight4


if(maxweight != 0) then (
      if(bone1Off != -1) then (
         w1 = weight1 as float
         append w.boneids (Bone1)
         append w.weights (w1)
      )
      if(bone2Off != -1) then (
         w2 = weight2 as float
         append w.boneids (Bone2)
         append w.weights (w2)
      )
      if(bone3Off != -1) then (
         w3 = weight3 as float
         append w.boneids (Bone3)
         append w.weights (w3)
      )
      if(bone4Off != -1) then (
         w4 = weight4 as float
         append w.boneids (Bone4)
        append w.weights (w4)
      )         
   )
append Weight_array w		
)	

print WeightsStart

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

max modify mode
select msh
skinMod = skin ()
addModifier msh skinMod
for i = 1 to BoneMap.count do
(
	maxbone = getnodebyname BoneMap[i]
	if i != BoneMap.count then 
		skinOps.addBone skinMod maxbone 0
	else
		skinOps.addBone skinMod maxbone 1
)

modPanel.setCurrentObject skinMod
for i = 1 to VertexCount do (
   w = Weight_array[i]
   bi = #() --bone index array
   wv = #() --weight value array
   
   for j = 1 to w.boneids.count do
   (
      boneid = w.boneids[j]
      weight = w.weights[j]
      append bi boneid
      append wv weight
   )   
   
   skinOps.ReplaceVertexWeights skinMod i bi wv
)
)catch(format "Error!!!\n")
)	

Print ("Mesh End@ 0x"+((bit.intAsHex(ftell f))as string))
)

fclose f	