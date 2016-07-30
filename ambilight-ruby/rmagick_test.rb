require 'rmagick'
include Magick

def printAllPictures()
	$top.write("top.png")
	$bottom.write("bottom.png")
	$left.write("left.png")
	$right.write("right.png")
	$fullCapture.write("all.png")
end

start = Time.now

#$fullCapture = Image::capture(true) { self.filename="root" }
`scrot screen.png`
$fullCapture = ImageList.new("screen.png")

capture = Time.now

$top = $fullCapture.crop(GravityType::NorthGravity, $fullCapture.columns, 100).adaptive_resize(60, 1) 

singlecrop = Time.now

$bottom = $fullCapture.crop(GravityType::SouthGravity, $fullCapture.columns, 100).adaptive_resize(60, 1)
$left = $fullCapture.crop(GravityType::NorthWestGravity, 100, 1280).adaptive_resize(1, 30) 
$right = $fullCapture.crop(GravityType::NorthEastGravity, 100, 1920).adaptive_resize(1, 30)

allcrop = Time.now

printAllPictures()

write = Time.now

print "alltime:" + ((allcrop - start).to_s) + "\n"
print "singlecrop:" + ((singlecrop - capture).to_s)+ "\n"
print "allcrop:" + ((allcrop - capture).to_s)+ "\n"
print "capture:" + ((capture - start).to_s)+ "\n"
print "write:" + ((write - allcrop).to_s) + "\n"
exit
