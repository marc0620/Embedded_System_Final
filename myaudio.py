
import pyaudio
import wave
import socket
import time
import timeit
import numpy


# s = socket.socket()
# host = '172.20.10.3'
# print (host)
# port = 80
# s.bind((host,port))
# s.listen(5)

# print("Connecting....")
# # break
# c,addr = s.accept()
# print ("Got connection from",addr)
# c.recv(1024)
# c.send(b"mmmmmmmmmmmmmmmmmmmmmmmmmmm1Hello from Rpiasdnkfjask ja[opsfp")

# def translate(value, leftMin, leftMax, rightMin, rightMax):
#     # Figure out how 'wide' each range is
#     leftSpan = leftMax - leftMin
#     rightSpan = rightMax - rightMin

#     # Convert the left range into a 0-1 range (float)
#     valueScaled = float(value - leftMin) / float(leftSpan)

#     # Convert the 0-1 range into a value in the right range.
#     return rightMin + (valueScaled * rightSpan)


form_1 = pyaudio.paInt16 # 16-bit resolution
chans = 1 # 1 channel
samp_rate = 4096 # 44.1kHz sampling rate
chunk = 1024 # 2^12 samples for buffer
record_secs = 5 # seconds to record
dev_index = 1 # device index found by p.get_device_info_by_index(ii)
wav_output_filename = 'test3.wav' # name of .wav file


audio = pyaudio.PyAudio() # create pyaudio instantiation
start = timeit.default_timer()

# create pyaudio stream
stream = audio.open(format = form_1,rate = samp_rate,channels = chans, \
                            input_device_index = dev_index,input = True, \
                                                frames_per_buffer=chunk)
print("recording")
frames = []

#loop through stream and append audio chunks to frame array
# for ii in range(0,int((samp_rate/chunk)*record_secs)):
#     data = (stream.read(chunk,exception_on_overflow = False))
    # frames.append(data)
    # print(data)
    # for each in data:
    #     print (type(each),each)
    #     int_temp = int.from_bytes(data,"big")
    #     float_temp = translate(each,0,256,-128,127)
    #     print (type(float_temp))
    #     print (float_temp)
    #     packNo = struct.pack('f',float_temp)
    #     print (type(packNo),packNo)
    #     frames.append(packNo)


    # print (data)
    # frames.append(data)
    # # s.send(data)
    # print (frames)

# while True:
#     print ("start sending")
for ii in range(0,int((samp_rate/chunk)*record_secs)):
    data = stream.read(chunk,exception_on_overflow = False)
    frames.append(data)
    # temp = array.array('h')
    print (type(data))
    temp = numpy.fromstring(data,'int16')
    print(temp)
    # c.send(temp)
    del temp
    del data

    # temp.frombytes(' '.join(data).encode('utf-8'))
    # for j in range(0,16):
    # c.send(data)
    # print (data)
    # print(numpy.fromstring(data,'int8'))
    # print()
    print ("-----------a chunk send-----------")
stop = timeit.default_timer()
    # del data

print("Finish calculating" , stop-start)



# stop the stream, close it, and terminate the pyaudio instantiation
stream.stop_stream()
stream.close()
audio.terminate()

# save the audio frames as .wav file
wavefile = wave.open(wav_output_filename,'wb')
wavefile.setnchannels(chans)
wavefile.setsampwidth(audio.get_sample_size(form_1))
wavefile.setframerate(samp_rate)
wavefile.writeframes(b''.join(frames))
wavefile.close()

