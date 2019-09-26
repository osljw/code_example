import tensorflow as tf
tf.enable_eager_execution()

from captcha.image import ImageCaptcha
import numpy as np
import random
import string

characters = string.digits + string.ascii_uppercase

n_len = 4 # max num of captcha
width, height = 128, 64

def captcha_gen(batch_size=1):
    X = np.zeros((batch_size, height, width, 3), dtype=np.uint8)
    y = np.zeros((batch_size, n_len), dtype=np.uint8)
    #input_length = np.ones(batch_size) * int(height-2)
    input_length = np.ones(batch_size) * 16
    label_length = np.ones(batch_size) * n_len
    generator = ImageCaptcha(width=width, height=height)
    while True:
        for i in range(batch_size):
            random_str = ''.join([random.choice(characters) for j in range(4)])
            #X[i] = np.array(generator.generate_image(random_str)).transpose(1, 0, 2)
            X[i] = generator.generate_image(random_str)
            y[i] = [characters.find(x) for x in random_str]
        #yield ((X, input_length, label_length), y)
        yield (X, y, input_length, label_length), 1

def captcha_gen_one():
    #input_length = int(height-2)
    input_length = [16]
    label_length = [n_len]
    generator = ImageCaptcha(width=width, height=height)
    i = 1
    while i <=30:
        i += 1
        random_str = ''.join([random.choice(characters) for j in range(4)])
        X = np.array(generator.generate_image(random_str)) / 255.0
        y = [characters.find(x) for x in random_str]
        yield (X, input_length, label_length), y

def captcha_data(length=5):
    def captcha_gen_one():
        #input_length = int(height-2)
        input_length = [16]
        label_length = [n_len]
        generator = ImageCaptcha(width=width, height=height)
        i = 1
        while i <=length:
            i += 1
            random_str = ''.join([random.choice(characters) for j in range(4)])
            X = np.array(generator.generate_image(random_str)) / 255.0
            y = [characters.find(x) for x in random_str]
            yield (X, input_length, label_length), y
    return captcha_gen_one

#def captcha_gen_one():
#    i = 0
#    while True:
#        i += 1
#        x = [i+1, i+2, i+3]
#        y = i
#        yield (x, y)
#
#def captcha_gen_one():
#    i = 0
#    while True:
#        i += 1
#        x = [i+1, i+2, i+3]
#        y = i
#        yield {"X":x, "Y":y}

@tf.function
def test_data():
    ds = tf.data.Dataset.from_generator(captcha_gen_one, ((tf.uint8, tf.int64, tf.int64), tf.int64)).batch(2)
    for i in ds:
        print(i)


if __name__ == '__main__':
    test_data()
    #data_shape = ((tf.TensorShape([None,  80, 170, 3]), tf.TensorShape([None, 4]), tf.TensorShape([None]), tf.TensorShape([None])), tf.TensorShape([]))
    #ds = tf.data.Dataset.from_generator(captcha_gen, ((tf.uint8, tf.uint8, tf.int64, tf.int64), 1), data_shape)
    
    data_shape = (((80, 170, 3), (4), (), ()), ())
    #ds = tf.data.Dataset.from_generator(captcha_gen_one, ((tf.uint8, tf.uint8, tf.int64, tf.int64), tf.int64), data_shape).batch(2)
    ds = tf.data.Dataset.from_generator(captcha_gen_one, ((tf.uint8, tf.int64, tf.int64), tf.int64)).batch(2)
    val_ds = tf.data.Dataset.from_generator(captcha_gen_one, ((tf.uint8, tf.int64, tf.int64), tf.int64)).batch(2)
    #ds = tf.data.Dataset.from_generator(captcha_gen_one, {"X":tf.int64, "Y":tf.int64}).batch(2)
    print(next(iter(ds)))
    print(next(iter(val_ds)))
    #it = iter(ds)
    #print(next(it), next(it))

    #for i in ds:
    #    print(i)
