var sheet_id = "1vIw96Cwi1vrcRgBjIOD_3L8Ys1TyAdbt0UwBQMRiIxo";
var ss = SpreadsheetApp.openById(sheet_id);

var log_sheet_name = "Serre_ESP32_logs";
var sheet_log = ss.getSheetByName(log_sheet_name);

var conf_sheet_name = "ESP32_conf";
var sheet_conf = ss.getSheetByName(conf_sheet_name);

function doPost(e){
  var image = e.parameter.image;
  
  if(image == undefined){
    var timestamp = String(e.parameter.timestamp);
    var temperature = Number(e.parameter.temperature);
    var humidity = Number(e.parameter.humidity);
    var light = Number(e.parameter.light);
    sheet_log.appendRow([timestamp, temperature, humidity, light]);
  }
  else{
    var data = Utilities.base64Decode(e.parameters.data);
    var nombreArchivo = Utilities.formatDate(new Date(), "GMT-3", "yyyyMMdd_HHmmss")+".jpg";
    var blob = Utilities.newBlob(data, e.parameters.mimetype, nombreArchivo );

    // Save the photo to Google Drive
    var folder, folders = DriveApp.getFoldersByName("ESP32-CAM");
    if (folders.hasNext()) {
      folder = folders.next();
    } else {
      folder = DriveApp.createFolder("ESP32-CAM");
    }
    var file = folder.createFile(blob);
  }
  
}

function doGet(e){
  var read = e.parameter.read;

  if(read != undefined){
    var jo = {};
    jo.DUREE_DEEPSLEEP = sheet_conf.getRange('B2').getValue();
    jo.LED = sheet_conf.getRange('B3').getValue();
    jo.MAX_HUMIDITY = sheet_conf.getRange('B4').getValue();
    jo.MIN_HUMIDITY = sheet_conf.getRange('B5').getValue();
    jo.MAX_TEMP = sheet_conf.getRange('B6').getValue();
    jo.MIN_TEMP = sheet_conf.getRange('B7').getValue();
    jo.FORCE_POMPE = sheet_conf.getRange('B8').getValue();
    jo.ARROSAGE_AUTOMATIQUE = sheet_conf.getRange('B9').getValue();
    jo.DUREE_AROSAGE = sheet_conf.getRange('B10').getValue();
    jo.HEURE_AROSAGE = sheet_conf.getRange('B11').getValue();
    jo.TIMELAPSE = sheet_conf.getRange('B12').getValue();
    jo.FREQUENCE_TIMELAPSE = sheet_conf.getRange('B13').getValue();
    jo.FORCE_PHOTO = sheet_conf.getRange('B14').getValue();
    jo.LED_PHOTO = sheet_conf.getRange('B15').getValue();


    result = JSON.stringify(jo);
    return ContentService.createTextOutput(result).setMimeType(ContentService.MimeType.JSON);
  }
  else{
    
  }
}

