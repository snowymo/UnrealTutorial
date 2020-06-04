/* SCE CONFIDENTIAL
 $PSLibId$
 * Copyright (C) 2013 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

function createXMLHttpRequest(cbFunc)
{
	var XMLhttpObject = null;
	try{
		XMLhttpObject = new XMLHttpRequest();
	}catch(e){
		try{
			XMLhttpObject = new ActiveXObject("Msxml2.XMLHTTP");
		}catch(e){
			try{
				XMLhttpObject = new ActiveXObject("Microsoft.XMLHTTP");
			}catch(e){
				return null;
			}
		}
	}
	if (XMLhttpObject) XMLhttpObject.onreadystatechange = cbFunc;
	return XMLhttpObject;
}
