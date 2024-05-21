import numpy
import cv2

cap = cv2.VideoCapture("bad_apple.webm")
ret, frame = cap.read()

frames = []


final_w = 80

def convert_color_to_char(color):
	if color == 0:
		return " "
	elif color < 50:
		return "."
	elif color < 100:
		return ":"
	elif color < 150:
		return "o"
	elif color < 200:
		return "O"
	else:
		return "0"
	

with open('./result.txt', 'w') as file:
	while(1):
		ret, frame = cap.read()

		if (ret == False):
			break

		grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		grey_resized = cv2.resize(grey, (int(grey.shape[1] * (final_w / grey.shape[0])), final_w//2))
		# th3 = cv2.adaptiveThreshold(grey,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
		# cv2.THRESH_BINARY,11,2)
		frame = [[convert_color_to_char(value) for value in line] for line in grey_resized]
		# print(frames)
	# 	print(frame.)
	#    cv2.imshow('frame',frame)
	#    if cv2.waitKey(1) & 0xFF == ord('q') or ret==False :
	#        cap.release()
	#        cv2.destroyAllWindows()
	#        break
	#    cv2.imshow('frame',frame)
		result = ""
		for line in frame:
			result += "".join(line)
			result += "\n"
		print(result)
		result += "---\n"
		file.write(result)






# for frame in frames:
# 	for line in frame:
# 		result += "".join(line)
# 		result += "\n"
# 	result += "---\n"






# 	file.write(result)

