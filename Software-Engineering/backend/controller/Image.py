import json
from flask import *
from flask_cors import *
from controller import bluep
from model import ImageOperationObject, session

@bluep.route('/images', methods=['GET', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def send_image():
    """发送图片给客户端"""
    json_data = request.args.to_dict()
    image_id = json_data['image_id']
    with open(r"../images/" + image_id + ".jpg", 'rb') as f:
        image = f.read()
        return Response(image, mimetype="image/png")
    # return redirect("../images/" + image_id + ".jpg")
    # return bluep.send_static_file("../images/" + image_id + ".jpg")

@bluep.route('/upload_image', methods=['POST', 'OPTIONS'])
@cross_origin(supports_credentials=True)
def upload_image():
    """接受图片"""
    upload_file = request.files['image']
    if upload_file is None:
        return {'isSuccess': False, 'data': None, 'message': '没收到图片'}
    msg = ImageOperationObject.create_image()
    if msg['err'] is not None:
        session.rollback()
        return {'isSuccess': False, 'data': None, 'message': msg['err']}
    session.commit()
    image_id = str(msg['result'])
    file_path = '../images/' + image_id + '.jpg'
    upload_file.save(file_path)
    return {'isSuccess': True, 'result': {'url': image_id}, 'message': "上传成功"}