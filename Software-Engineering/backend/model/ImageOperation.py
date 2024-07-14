from model.models import Images
from model import session

class ImageOperation:
    def create_image(self):
        new_image = Images({})
        try:
            session.add(new_image)
            session.flush()
            return {'result': new_image.image_id, 'err': None}
        except Exception as e:
            print(e)
            return {'result': None, 'err': str(e)}