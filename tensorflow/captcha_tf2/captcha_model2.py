import tensorflow as tf
tf.enable_eager_execution()
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Input, Conv2D, Reshape, Dense, GRU, Dropout, Activation
from tensorflow.keras.layers import Permute, TimeDistributed, Flatten
from tensorflow.keras.layers import Lambda, MaxPooling2D, Bidirectional, CuDNNGRU, BatchNormalization
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import Callback, ModelCheckpoint
import string
import os
import shutil

from captcha_gen2 import captcha_gen_one
from captcha_gen2 import captcha_data

width, height = 128, 64
characters = string.digits + string.ascii_uppercase
n_class = len(characters) + 1
n_len = 4

#def ctc_lambda_func(args):
#    y_pred, labels, input_length, label_length = args
#    return tf.keras.backend.ctc_batch_cost(labels, y_pred, input_length, label_length)

#def evaluate(model, batch_size=128, steps=20):
#    batch_acc = 0
#    valid_data = CaptchaSequence(characters, batch_size, steps)
#    for [X_test, y_test, _, _], _ in valid_data:
#        y_pred = base_model.predict(X_test)
#        shape = y_pred.shape
#        out = K.get_value(K.ctc_decode(y_pred, input_length=np.ones(shape[0])*shape[1])[0][0])[:, :4]
#        if out.shape[1] == 4:
#            batch_acc += (y_test == out).all(axis=1).mean()
#    return batch_acc / steps

test_ds = tf.data.Dataset.from_generator(captcha_gen_one, ((tf.float32, tf.int64, tf.int64), tf.int64)).batch(2)
test_input,test_label = next(iter(test_ds))

class Evaluate(Callback):
    def __init__(self):
        self.accs = []
    
    def on_epoch_end(self, epoch, logs=None):
        logs = logs or {}
        #acc = evaluate(model_infer)
        pred = model_infer.predict(test_input[0])
        logs['pred'] = pred
        self.accs.append(pred)
        print(f'\npred: {pred}\n\n')

class ModelSave(Callback):
    def __init__(self, filepath):
        pass
    
    def on_epoch_end(self, epoch, logs=None):
        if os.path.exists("model"):
            shutil.rmtree("model")
        tf.keras.experimental.export_saved_model(model_infer, 'model')

def ctc_loss_wrap(input_length, label_length):
    def ctc_loss(y_true, y_pred):
        y_true = tf.reshape(y_true, (-1, 4))
        return tf.keras.backend.ctc_batch_cost(y_true, y_pred, input_length, label_length)
    return ctc_loss


def build_model():
    input_tensor = Input((height, width, 3))
    x = input_tensor
    for i, n_cnn in enumerate([2, 2, 2, 2, 2]):
        for j in range(n_cnn):
            x = Conv2D(32*2**min(i, 3), kernel_size=3, padding='same', kernel_initializer='he_uniform')(x)
            x = BatchNormalization()(x)
            x = Activation('relu')(x)
        x = MaxPooling2D(2 if i < 3 else (2, 1))(x)
    
    x = Permute((2, 1, 3))(x)
    x = TimeDistributed(Flatten())(x)
    print("=====TimeDistributed:", x.get_shape())
    
    #print("========",x.get_shape())
    #timestamp = int(x.get_shape()[1])
    #dim = int(x.get_shape()[2]) * int(x.get_shape()[3])
    #x = Reshape((timestamp, dim))(x)
    #print("=====reshape:", x.get_shape())
    
    rnn_size = 128
    x = Bidirectional(GRU(rnn_size, return_sequences=True))(x)
    x = Bidirectional(GRU(rnn_size, return_sequences=True))(x)
    x = Dense(n_class, activation='softmax')(x)
    print("========x:", x)
    
    
    #labels = Input(name='the_labels', shape=[n_len], dtype='int64')
    input_length = Input(name='input_length', shape=[1], dtype='int64')
    label_length = Input(name='label_length', shape=[1], dtype='int64')
    model = Model(inputs=[input_tensor, input_length, label_length], outputs=x)

    def func(x):
        print("========decode:", x)
        #input_length = tf.reshape(tf.ones_like(x[:,0, 0]), [-1]) * 16
        #input_length = tf.ones(tf.shape(x)[0]) * int(x.shape[1])
        input_length = tf.fill(tf.reshape(tf.shape(x)[0], [-1]), tf.constant(int(x.shape[1])))
        #input_length = tf.ones(2) * 16
        print("========decode2:", input_length)
        return tf.keras.backend.ctc_decode(x, input_length)
    output_pred = tf.keras.layers.Lambda(func)(x)
    print("======ctc_decode:",output_pred)
    model_infer = Model(inputs=input_tensor, outputs=output_pred[0])
    
    model.compile(loss=ctc_loss_wrap(input_length, label_length), optimizer=Adam(1e-3, amsgrad=True))
    return model, model_infer

if __name__ == '__main__':
    model, model_infer = build_model()
    #model.summary()
    model_infer.summary()
    #model.save_weights("before/before_model")
    #tf.keras.experimental.export_saved_model(model, 'saved_model_train') # failed

    #import shutil
    #shutil.rmtree("saved_model")
    #tf.keras.experimental.export_saved_model(model_infer, 'saved_model')
    ##model_infer.save("saved_model", save_format="tf")

    
    #data_shape = (((128, 64, 3), (1), (1)), ())
    #ds = tf.data.Dataset.from_generator(captcha_gen_one, ((tf.uint8, tf.int64, tf.int64), tf.int64), data_shape).batch(2)
    batch_size = 128
    train_data_len = batch_size * 10
    val_data_len = batch_size * 5
    train_ds = tf.data.Dataset.from_generator(captcha_data(train_data_len), ((tf.float32, tf.int64, tf.int64), tf.int64)).batch(batch_size)
    val_ds = tf.data.Dataset.from_generator(captcha_data(val_data_len), ((tf.float32, tf.int64, tf.int64), tf.int64)).batch(batch_size)
    #test_input,test_label = next(iter(val_ds))
    print(test_input, "label:", test_label)
    print("=======predict:")
    pred = model_infer.predict(test_input[0])
    print("label:", test_label)
    print("======pred:", pred)
    #model.fit(train_ds, validation_data=val_ds, steps_per_epoch=20)
    #callbacks = [Evaluate(), ModelSave("model/weights.{epoch:02d}-{val_loss:.2f}.h5")]
    if not os.path.exists("model"):
        os.mkdir("model")

    latest_model = tf.train.latest_checkpoint("model")
    if latest_model is not None:
        print("=====load model from:", latest_model)
        model.load_weights(latest_model)
    # save_weights_only=False -> Could not pack sequence. Structure had 3 elements, but flat_sequence had 1 elements.
    callbacks = [Evaluate(), ModelCheckpoint("model/weights.{epoch:02d}-{val_loss:.2f}", save_weights_only=True)]
    model.fit(train_ds, validation_data=val_ds, epochs=5, callbacks=callbacks)
    #model.fit(train_ds, epochs=2, callbacks=callbacks)
    model.save_weights("after/after_model")
    pred = model_infer.predict(test_input[0])
    print("======pred2:", pred)
    import shutil
    import os
    if os.path.exists("saved_model"):
        shutil.rmtree("saved_model")
    tf.keras.experimental.export_saved_model(model_infer, 'saved_model')
