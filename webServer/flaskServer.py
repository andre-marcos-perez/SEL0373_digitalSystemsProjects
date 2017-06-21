from flask import Flask, session, Response, render_template, request, jsonify
from camera import Camera
from mqtt import Mqtt


flaskServer = Flask(__name__)
flaskServer.secret_key = 'random key'

# inicio
@flaskServer.route('/')
def webRender_inicio():
    return render_template('inicio.html')

# sobre nos
@flaskServer.route('/sobreNos/')
def webRender_sobreNos():
    return render_template('sobreNos.html')

# login
@flaskServer.route('/login/', methods=['GET', 'POST'])
def webRender_login():
    error = None
    if request.method == 'POST':
        if request.form['userName'] != 'admin':
            error = 'Usuario ou senha invalidos!'
	elif request.form['userPassword'] != 'admin':
            error = 'Usuario ou senha invalidos!'
        else:
            session['is_logged'] = True
            return render_template('sistema.html')
    return render_template('login.html', error = error)

# cadastro
@flaskServer.route('/cadastro/', methods=['GET', 'POST'])
def webRender_cadastro():
    error = None
    if request.method == 'POST':
        if request.form['userName'] != 'admin':
            error = 'Usuario invalido!'
        elif request.form['userPassword'] != 'admin':
            error = 'Senha invalida!'
        elif request.form['userEmail'] != 'admin@usp.br':
            error = 'E-mail invalido!'
        elif request.form['deviceId'] != '123':
            error = 'Chave do dispositivo invalido!'
        else:
            error = 'Cadastro realizado com sucesso'
            return render_template('login.html', error = error)
    return render_template('cadastro.html', error = error)

# sobre nos
@flaskServer.route('/sistema/', methods=['GET', 'POST'])
def webRender_sistema():
    if request.method == 'POST':
        session.pop('is_logged', None)
        return render_template('login.html', error = None)        
    return render_template('sistema.html')

# videoStreaming
@flaskServer.route('/videoStreaming/')
def videoStreaming():
    return Response(gen(Camera()), 
                    mimetype='multipart/x-mixed-replace; boundary=frame')


# liveCamera
@flaskServer.route('/liveCamera/', methods=['GET', 'POST'])
def webRender_liveCamera():	
    if request.method == 'POST':
        if request.form['direction'] == 'up':
            Camera.move_camera('up')
        if request.form['direction'] == 'down':
            Camera.move_camera('down')
        if request.form['direction'] == 'left':
            Camera.move_camera('left')
        if request.form['direction'] == 'right':
            Camera.move_camera('right')
    return render_template('liveCamera.html')


def gen(camera):

    #while True:
    frame = camera.get_frame()
    yield (b'--frame\r\n'
           b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

# liveLocker
@flaskServer.route('/liveLocker/', methods=['GET', 'POST'])
def webRender_liveLocker():
    
    return render_template('liveLocker.html')

# mqttData
@flaskServer.route('/getData/', methods=['GET'])
def getData():

    myMqtt = Mqtt()
    mqttData = myMqtt.printPayload()
    return jsonify(mqttData = mqttData)

if __name__ == '__main__':
    flaskServer.run(host='192.168.1.111', port=8186, debug='TRUE')
