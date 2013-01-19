#!/usr/bin/env python

from lxml import etree, html
from urllib2 import Request, urlopen
import csv

POST_URL="http://ec2-50-19-164-82.compute-1.amazonaws.com:8080/solr/core0/update?commit=true"

def loadCraig():
    craigDict = dict()
    with open('craig.csv') as craig:
        craigKV = csv.reader(craig)
        for row in craigKV:
            craigDict[row[0]] = row[1]
    return craigDict

def storeCraig(craigDict):
    with open('craig.csv', 'w') as craig:
        craigWriter = csv.writer(craig)
        for k, v in craigDict.iteritems():
            craigWriter.writerow([k,v])
    return

craigDict = loadCraig()

CRAIG_ADDR = "http://www.craigslist.org/about/sites"
craig_response = urlopen(CRAIG_ADDR)
craig_root = html.parse(craig_response)

us = craig_root.xpath('//div[@class="colmask" and h1/a[@name="US"]]')
if us != []:
    cities = us[0].xpath('.//li/a/@href')
    for city in cities:
        
        try:
            lastForCity = craigDict[city]
        except:
            lastForCity = ""
        
        addr = city + "/bia"
        cityName = city.split('/')[2].split('.')[0]    
        city_response = urlopen(addr)
        city_root = html.parse(city_response)
        rows = city_root.xpath('//p[@class="row"]')
        if rows != []:
            url = rows[0].xpath('a/@href')[0]
            newLast = url.split('/')[-1].split('.')[0]
        else:
            newLast = lastForCity
            
        globalRoot = etree.Element('add')

        for row in rows[:10]:

            url = row.xpath('a/@href')[0]
            postId = url.split('/')[-1].split('.')[0]
            
            if postId == lastForCity:
                break

            print "Parsing..." + postId

            row_response = urlopen(url)
            row_root = html.parse(row_response)

            bodyL = row_root.xpath('//section[@id="postingbody"]')
            if bodyL != []:
                body = bodyL[0].text
            else:
                body = ""

            imgUrl = row_root.xpath('//div[@id="thumbs"]/a/@href')

            dateL = row_root.xpath('//date')
            if dateL != []:
                date = dateL[0].text
            else: 
                date = ""

            locationL = row_root.xpath('//ul[@class="blurbs"]/li')
            location = cityName
            try:
                locL = locationL[0].text_content().split(' ')
                if locL[1] == "Location:":
                    location = cityName + "; " + ' '.join(locL[2:])
            except:
                location = cityName

            adRoot = etree.Element('doc')
            
            postIdNode = etree.Element('field', name='postId')
            postIdNode.text = postId
            adRoot.append(postIdNode)
            
            dateIdNode = etree.Element('field', name='date')
            dateIdNode.text = date
            adRoot.append(dateIdNode)

            imgUrlNode = etree.Element('field', name='imageURL')
            if imgUrl != []:
                imgUrlNode.text = imgUrl[0]
            adRoot.append(imgUrlNode)

#            imgUrlsNode = etree.Element('imgUrls')
#            for imgU in imgUrl:
#                imgUNode = etree.Element('imgURL')
#                imgUNode.text = imgU
#                imgUrlsNode.append(imgUNode)
#            adRoot.append(imgUrlsNode)

            bodyNode = etree.Element('field', name='body')
            bodyNode.text = body
            adRoot.append(bodyNode)

            locationNode = etree.Element('field', name='location')
            locationNode.text = location
            adRoot.append(locationNode)

            globalRoot.append(adRoot)

        print "done with city"
        cityXML = etree.tostring(globalRoot)
        req = Request(url=POST_URL,
                              data=cityXML,
                              headers={'Content-Type': 'application/xml'})
        post_response = urlopen(req)
        print post_response

        craigDict[city] = newLast
        storeCraig(craigDict)
    
storeCraig(craigDict)
