//↓同じhファイルをincludeしないようにする記述。
#ifndef BERA_FACE_H_
#define BERA_FACE_H_

//↓必要なライブラリを読み込む
#include <M5Unified.h>
#include "BoundingRect.h"
#include "DrawContext.h"
#include "Drawable.h"
#include "Face.h"
#include "Expression.h"

//画像データを配列にしたものをそれぞれのヘッダーファイルで宣言している
#include "imgEyeL1.h"
#include "imgEyeR1.h"
#include "imgMouth1.h"
#include "imgHige.h"

namespace m5avatar {  //m5avatarライブラリと同じ場所にある体でプログラムを書くよ

//目のパーツを描画するクラス
class BeraEye : public Drawable { //Javaで言うところの  public class BeraEye extends Drawable { 
  private:
  bool isLeft;  //privateなフィールド
  uint8_t drawCount;
  LGFX_Sprite sprGuru;

  public:
  BeraEye(bool isLeft):isLeft{isLeft} {//ここは引数付きコンストラクタ
    drawCount = 0;
    sprGuru.createSprite(imgGuruWidth,imgGuruHeight);
    sprGuru.setPivot(imgGuruWidth/2,imgGuruHeight/2);
    sprGuru.drawBitmap(0,0,epd_bitmap_guruguru,imgGuruWidth,imgGuruHeight,WHITE);
  } 

  //drawメソッドをオーバーライドすることで、描画時の動作を変える。
  void draw(M5Canvas *spi, BoundingRect rect, DrawContext *ctx) {
    //ここは、現在描画中の様々な情報を取得する部分
    uint32_t cx = rect.getCenterX();
    uint32_t cy = rect.getCenterY();
    Gaze g = ctx->getGaze();
    ColorPalette *cp = ctx->getColorPalette();
    uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : cp->get(COLOR_PRIMARY);
    uint16_t backgroundColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : cp->get(COLOR_BACKGROUND);
    uint32_t offsetX = g.getHorizontal() * 8;
    uint32_t offsetY = g.getVertical() * 5;
    float eor = ctx->getEyeOpenRatio();
    float b = ctx->getBreath();

    //感情取得
    auto exp = ctx->getExpression();
    if(exp == Expression::Angry){//怒っていたら
      if(isLeft){//画面に向かって左側ならtrue
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL3, imgEye1Width, imgEye1Height, primaryColor);
      }else{
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR3, imgEye1Width, imgEye1Height, primaryColor);
      }
      return;
    }
    if(exp == Expression::Happy){
      if(isLeft){//画面に向かって左側ならtrue
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL4, imgEye1Width, imgEye1Height, primaryColor);
      }else{
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR4, imgEye1Width, imgEye1Height, primaryColor);
      }
      return;
    }
    if(exp == Expression::Sleepy){
      if(isLeft){//画面に向かって左側ならtrue
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL5, imgEye1Width, imgEye1Height, primaryColor);
      }else{
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR5, imgEye1Width, imgEye1Height, primaryColor);
      }
      return;
    }
    if(exp == Expression::Doubt){
      float kakudo = (360 / 10) * (drawCount % 10);
      if(isLeft){//画面に向かって左側ならtrue
        //白目部分
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL1, imgEye1Width, imgEye1Height, primaryColor);
        //黒目部分
        spi->fillEllipse(cx +17, cy +5, 35, 35,backgroundColor);
        //if(b<0)b=0;
        sprGuru.pushRotateZoom(spi,cx +17,cy +5,kakudo,1,1,BLACK);
      }else{
        //白目部分
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR1, imgEye1Width, imgEye1Height, primaryColor);
        spi->fillEllipse(cx -16, cy, 35, 35,backgroundColor);
        //if(b<0)b=0;
        sprGuru.pushRotateZoom(spi,cx -16,cy,kakudo,1,1,BLACK);
      }
      drawCount++;
      return;
    }
    if(exp == Expression::Sad){
      if(isLeft){//画面に向かって左側ならtrue
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL6, imgEye1Width, imgEye1Height, primaryColor);
      }else{
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR6, imgEye1Width, imgEye1Height, primaryColor);
      }
      return;
    }

    //以下、実際に別の顔を描画する部分(ここはBeraEyeクラスなので目だけ！)
    if(isLeft){//画面に向かって左側ならtrue
      if (eor == 0) {//目を閉じていたら
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL2, imgEye1Width, imgEye1Height, primaryColor);
      }else{
        //白目部分
        spi->drawXBitmap(
        cx - imgEye1Width / 2 + 20,
        cy - imgEye1Height / 2 - 10,
        imgEyeL1, imgEye1Width, imgEye1Height, primaryColor);
        //黒目部分
        spi->fillEllipse(cx +17 +offsetX, cy +5 +offsetY, 35, 35,backgroundColor);
      }
    }else{
      if (eor == 0) {//目を閉じていたら
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR2, imgEye1Width, imgEye1Height, primaryColor);
      }else{
        spi->drawXBitmap(
        cx - imgEye1Width / 2 - 20,
        cy - imgEye1Height / 2 - 15,
        imgEyeR1, imgEye1Width, imgEye1Height, primaryColor);
        spi->fillEllipse(cx -16 +offsetX, cy +offsetY, 35, 35,backgroundColor);
      }
    }
  }

};


//口パーツを描画するクラス
class BeraMouth : public Drawable {
  private:
  LGFX_Sprite sprHana;
  public:
  BeraMouth(){
    sprHana.createSprite(imgHanatyoWidth,imgHanatyoHeight);
    sprHana.setPivot(imgHanatyoWidth,imgHanatyoHeight);
    sprHana.drawBitmap(0,0,epd_bitmap_hanatyo,imgHanatyoWidth,imgHanatyoHeight,WHITE);
  } //ここは引数付きコンストラクタ

  void draw(M5Canvas *spi, BoundingRect rect, DrawContext *ctx) {
    uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
    uint16_t backgroundColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
    uint32_t cx = rect.getCenterX();
    uint32_t cy = rect.getCenterY();
    float openRatio = ctx->getMouthOpenRatio();
    float b = ctx->getBreath();
    //感情取得
    auto exp = ctx->getExpression();
    if(exp == Expression::Happy){
      spi->drawXBitmap(
      cx - imgMouth2Width / 2,
      cy - imgMouth1Height / 2,
      imgMouth3, imgMouth2Width, imgMouth2Height, primaryColor);
    }
    if(exp == Expression::Sleepy){
      openRatio = 1;
    }
    //口描画
    if(openRatio == 0){
      spi->drawXBitmap(
      cx - imgMouth1Width / 2,
      cy - imgMouth1Height / 2,
      imgMouth1, imgMouth1Width, imgMouth1Height, primaryColor);
    }else{
      spi->drawXBitmap(
      cx - imgMouth2Width / 2,
      cy - imgMouth1Height / 2,
      imgMouth2, imgMouth2Width, imgMouth2Height, primaryColor);
    }
    if(exp == Expression::Sleepy){
      if(b<0.3)b=0.3;
      sprHana.pushRotateZoom(spi,cx,cy,0,b,b,BLACK);
    }


    //ひげ描画
    spi->drawXBitmap(
    10,
    cy - imgMouth1Height / 2 + 5,
    epd_bitmap_beraHige1, imgHigeWidth, imgHigeHeight, primaryColor);

    spi->drawXBitmap(
    cx*2 - imgHigeWidth -10,
    cy - imgMouth1Height / 2 + 5,
    epd_bitmap_beraHige2, imgHigeWidth, imgHigeHeight, primaryColor);

  }
};

class BeraFace : public Face {
 public:
  BeraFace()
      : Face(new BeraMouth(), new BoundingRect(168, 163), new BeraEye(true),
             new BoundingRect(103, 80), new BeraEye(false),
             new BoundingRect(106, 240), new Eyeblow(0, 0, false),
             new BoundingRect(67, 96), new Eyeblow(0, 0, true),
             new BoundingRect(72, 230)) {}
};

}  // namespace m5avatar

#endif  // BERA_FACE_H_
