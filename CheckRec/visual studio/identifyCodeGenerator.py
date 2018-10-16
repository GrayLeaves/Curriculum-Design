	
from PIL import Image,ImageDraw,ImageFont,ImageFilter
import random

 #identify code default length=1
def random_code(lenght=1):    
    code = ''
    for char in range(lenght):
        code += chr(random.randint(65,90))
    return code

	#random color range(1,255)
def random_color(s=1,e=255):
    return (random.randint(s,e),random.randint(s,e),random.randint(s,e))
	#generater code picture
	#length code length
	#width code wild
	#height code heigth
	#return the picture
def veri_code(lenght=4, width=160, height=40):
    #create image object
    image = Image.new('RGB',(width,height),(255,255,255))
    #create font object
    font = ImageFont.truetype('arial.ttf',32)
    #create draw object
    draw = ImageDraw.Draw(image)
    #fill ever pixels with random color
    for x in range(width):
        for y in range(height):
            draw.point((x,y),fill=random_color(64,255))
    #identifying code
    code = random_code(lenght)
    #write the random color identifying code on the picture
    for t in range(lenght):
        draw.text((40*t+5,5),code[t],font=font,fill=random_color(32,127))
    #average filter
    image = image.filter(ImageFilter.BLUR)
    return code,image

def main():
    for i in range(10):
        code, image = veri_code()
        #image.show()
        image.save('C:\\Users\\wuzhenguang\\Pictures\\' + str(i) + '.jpg')

main()