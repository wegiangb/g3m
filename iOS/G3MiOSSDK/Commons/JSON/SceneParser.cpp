//
//  SceneParser.cpp
//  G3MiOSSDK
//
//  Created by Eduardo de la Montaña on 15/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "SceneParser.hpp"

#include "IJSONParser.hpp"
#include "IStringBuilder.hpp"
#include "IStringUtils.hpp"

#include "JSONBaseObject.hpp"
#include "JSONNumber.hpp"
#include "JSONObject.hpp"
#include "JSONArray.hpp"
#include "JSONString.hpp"
#include "JSONBoolean.hpp"

#include "WMSLayer.hpp"
#include "LayerSet.hpp"

#include <stdio.h>
#include <string.h>

#include "MarksRenderer.hpp"
#include "GEOJSONDownloadListener.hpp"

using namespace std;

const std::string SceneParser::LAYERS = "layers";
const std::string SceneParser::TYPE = "type";
const std::string SceneParser::DATASOURCE = "datasource";
const std::string SceneParser::VERSION = "version";
const std::string SceneParser::ITEMS = "items";
const std::string SceneParser::STATUS = "status";
const std::string SceneParser::NAME = "name";
const std::string SceneParser::URLICON = "urlIcon";


const std::string SceneParser::WMS110 = "1.1.0";
const std::string SceneParser::WMS111 = "1.1.1";
const std::string SceneParser::WMS130 = "1.3.0";

SceneParser* SceneParser::_instance = NULL;

SceneParser* SceneParser::instance(){
    if (_instance == NULL){
        _instance = new SceneParser();
    }
    return _instance;
}

SceneParser::SceneParser(){
    _mapLayerType["WMS"] = WMS;
    _mapLayerType["THREED"] = THREED;
    _mapLayerType["PANO"] = PANO;
    _mapLayerType["GEOJSON"] = GEOJSON;  
}

void SceneParser::parse(LayerSet* layerSet, std::string namelessParameter){
    
    _mapGeoJSONSources.clear();
    
    JSONObject* json = IJSONParser::instance()->parse(namelessParameter)->asObject();
    parserJSONLayerList(layerSet, json->getAsObject(LAYERS));
    IJSONParser::instance()->deleteJSONData(json);
}

void SceneParser::parserJSONLayerList(LayerSet* layerSet, JSONObject* jsonLayers){
    for (int i = 0; i < jsonLayers->size(); i++) {
        IStringBuilder* isb = IStringBuilder::newStringBuilder();
        isb->addInt(i);
        JSONObject* jsonLayer = jsonLayers->getAsObject(isb->getString());
        const layer_type layerType = _mapLayerType[jsonLayer->getAsString(TYPE)->value()];
        
        switch (layerType) {
            case WMS:
                parserJSONWMSLayer(layerSet, jsonLayer);
                break;
            case THREED:
                parserJSON3DLayer(layerSet, jsonLayer);
                break;
            case PANO:
                parserJSONPanoLayer(layerSet, jsonLayer);
                break;
            case GEOJSON:
                parserGEOJSONLayer(layerSet, jsonLayer);
                break;
        }
        
    }
}

void SceneParser::parserJSONWMSLayer(LayerSet* layerSet, JSONObject* jsonLayer){
    cout << "Parsing WMS Layer " << jsonLayer->getAsString(NAME)->value() << "..." << endl;
    
    const std::string jsonDatasource = jsonLayer->getAsString(DATASOURCE)->value();
    const int lastIndex = IStringUtils::instance()->indexOf(jsonDatasource,"?");
    const std::string jsonURL = IStringUtils::instance()->substring(jsonDatasource, 0, lastIndex+1);
    const std::string jsonVersion = jsonLayer->getAsString(VERSION)->value();
    
    JSONArray* jsonItems = jsonLayer->getAsArray(ITEMS);
    IStringBuilder *layersName = IStringBuilder::newStringBuilder();
    
    for (int i = 0; i<jsonItems->size(); i++) {
        if (jsonItems->getAsObject(i)->getAsBoolean(STATUS)->value()) {
            layersName->addString(jsonItems->getAsObject(i)->getAsString(NAME)->value());
            layersName->addString(",");
        }    
    }
    std::string layersSecuence = layersName->getString();
    if (layersName->getString().length() > 0) {
        layersSecuence = IStringUtils::instance()->substring(layersSecuence, 0, layersSecuence.length()-1);
    }
    
    //TODO check if wms 1.1.1 is neccessary to have it in account
    WMSServerVersion wmsVersion = WMS_1_1_0;
    if (jsonVersion.compare(WMS130)==0) {
        wmsVersion = WMS_1_3_0;
    }  
    
    WMSLayer* wmsLayer = new WMSLayer(URL::escape(layersSecuence),
                                      URL(jsonURL, false),
                                      wmsVersion,
                                      Sector::fullSphere(),
                                      "image/png",
                                      "EPSG:4326",
                                      "",
                                      true,
                                      NULL);
    layerSet->addLayer(wmsLayer);
}

void SceneParser::parserJSON3DLayer(LayerSet* layerSet, JSONObject* jsonLayer){
    cout << "Parsing 3D Layer " << jsonLayer->getAsString(NAME)->value() << "..." << endl;
}

void SceneParser::parserJSONPanoLayer(LayerSet* layerSet, JSONObject* jsonLayer){
    cout << "Parsing Pano Layer " << jsonLayer->getAsString(NAME)->value() << "..." << endl;
}

void SceneParser::parserGEOJSONLayer(LayerSet* layerSet, JSONObject* jsonLayer){
    cout << "Parsing GEOJSON Layer " << jsonLayer->getAsString(NAME)->value() << "..." << endl;
    
    const std::string geojsonDatasource = jsonLayer->getAsString(DATASOURCE)->value();
    
    JSONArray* jsonItems = jsonLayer->getAsArray(ITEMS);
    for (int i = 0; i<jsonItems->size(); i++) {
    
        const std::string namefile = jsonItems->getAsObject(i)->getAsString(NAME)->value();
        const std::string icon = jsonItems->getAsObject(i)->getAsString(URLICON)->value();
        
        IStringBuilder *url = IStringBuilder::newStringBuilder();
        url->addString(geojsonDatasource);
        url->addString("/");
        url->addString(namefile);
        
        _mapGeoJSONSources[url->getString()] = icon;
    }
}

std::map<std::string, std::string> SceneParser::getMapGeoJSONSources(){
    return _mapGeoJSONSources;   
}

