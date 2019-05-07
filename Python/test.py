import numpy as np
import cv2
import glob

class Calibration():
    def __init__(self, parent=None):
        self.calDone = False

    def doCalibration(self):
        print('doCalibration')
        chessSize = (10, 6)

        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

        objp = np.zeros((chessSize[0] * chessSize[1], 3), np.float32)
        objp[:, :2] = np.mgrid[0:chessSize[0], 0:chessSize[1]].T.reshape(-1, 2)

        objpoints = []  # 3d point in real world space
        imgpoints = []  # 2d points in image plane.

        images = glob.glob('/Users/chengong.cg/Git/AR/calibration/png/*.png')
        np.random.shuffle(images)
        images = images[0:20]
        images.sort()

        for fname in images:
            print(fname)
            img = cv2.imread(fname)
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

            ret, corners = cv2.findChessboardCorners(gray, chessSize, None)
            if ret:
                objpoints.append(objp)
                corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
                imgpoints.append(corners2)
                # img = cv2.drawChessboardCorners(img, chessSize, corners2, ret)
                # cv2.imshow('img', img)
                cv2.waitKey(1)

        ret, mtx, dist, rvecs, tvecs, devIn, devEx, err = cv2.calibrateCameraExtended(
            objpoints, imgpoints, gray.shape[::-1], None, None,
            flags=cv2.CALIB_FIX_ASPECT_RATIO | cv2.CALIB_FIX_K4 | cv2.CALIB_FIX_K5)

        print("default mtx", mtx)
        print("default dist", dist)

        h, w = img.shape[:2]
        print("image size", w, h)
        nmtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (w, h), 0.6, (w, h), centerPrincipalPoint=True)
        print("new mtx", nmtx)
        print("roi", roi)

        # cv2.destroyAllWindows()

        self.mapx, self.mapy = cv2.initUndistortRectifyMap(mtx, dist, None, mtx, (w, h), 5)
        self.calDone = True
        print('doCalibration done')

    def doUndistort(self):
        print('doUndistort')
        if (self.calDone == False):
             print('do Calibration first')
             return

        videos = ['/Users/chengong.cg/Git/AR/calibration/test/test.avi']
        for video in videos:
            cap = cv2.VideoCapture(video)
            height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
            width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
            fps = int(cap.get(cv2.CAP_PROP_FPS))
            fcnt = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
            print('height, width, fps, fcnt', height, width, fps, fcnt)
            mp4_filename = video[:-3] + 'mp4'
            out = cv2.VideoWriter(mp4_filename, cv2.VideoWriter_fourcc(*'mp4v'), fps, (width, height))
            no = 0
            delay = 0
            # cv2.namedWindow('orig', 0)
            # cv2.namedWindow('dst', 0)
            frame_exists, origframe = cap.read()
            while frame_exists:
                frame = origframe.copy()
                dst = cv2.remap(frame, self.mapx, self.mapy, cv2.INTER_LINEAR)
                out.write(dst)
                # cv2.imshow('orig', frame)
                # cv2.imshow('dst', dst)
                # keypress = 0xFF & cv2.waitKey(delay*100)
                # if keypress == ord('q') or keypress == 27:
                #     break
                # elif keypress == ord(' '):
                #     delay = 1 - delay
                frame_exists, origframe = cap.read()
                no += 1
        # cv2.destroyAllWindows()
        print('doUndistort done...', mp4_filename)

if __name__ == '__main__':
    np.set_printoptions(suppress=True)
    cal = Calibration()
    cal.doCalibration()
    cal.doUndistort()
    cv2.destroyAllWindows()