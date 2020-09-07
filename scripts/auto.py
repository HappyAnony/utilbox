#-*- coding:utf-8 -*-
#author:HappyAnony

import os
import sys
import difflib
import shutil
import chardet
import re
import yaml

#******************************************全局环境变量**************************************
# pclint的安装路径(window本地)
PCLINT_INSTALL_DIR = ""

# 项目头文件目录列表(写入include.lnt文件中, 被std.lnt文件加载)
PRO_INCLUDE_DIR_LIST     = []

# 项目名称
PRO_NAME = ""

# 待检查驱动源码目录名列表
DRV_SRC_DIR_LIST = []

# 待检查驱动源码文件名列表
DRV_SRC_FILE_LIST = []

# 待检查管理源码目录名列表
MNG_SRC_DIR_LIST = []

# 待检查管理源码文件名列表
MNG_SRC_FILE_LIST = []

# 上次pclint报告路径(本次对比对象)
LAST_PCLINT_REPORT_PATH = ""

# 驱动.c源码文件列表
DRVC_LIST = []
# 管理.c源码文件列表
MNGC_LIST = []

# 本次pclint报告生成路径(默认是c:\pclintReport)
CURRENT_PCLINT_REPORT_PATH = "c:\\pclintReport"
TMP_RESULT_FILE = ""

# 检查驱动源码标志
checkDrvFlag = False
# 检查管理源码标志
checkMngFlag = False
# 对比对象存在标志
cmpObjExistFlag = False


# 存放pclint待检查.c源码文件名
PRO_LNT_NAME     = "project.lnt"
PRO_LNT_PATH     = ""
# 存放pclint检查所依赖头文件路径名
INCLUDE_LNT_NAME = "include.lnt"
INCLUDE_LNT_PATH = ""
# 调用include.lnt文件的路径名
STD_LNT_NAME     = "std.lnt"
STD_LNT_PATH     = ""

# pclint报告相关
CUR_PCLINT_DRV_REPORT_FILE = ""
CUR_PCLINT_DRV_RESULT_DIR = ""
CUR_PCLINT_MNG_REPORT_FILE = ""
CUR_PCLINT_MNG_RESULT_DIR = ""

# 上次pclint报告相关
LAST_PCLINT_DRV_REPORT_FILE = ""
LAST_PCLINT_DRV_RESULT_DIR = ""
LAST_PCLINT_MNG_REPORT_FILE = ""
LAST_PCLINT_MNG_RESULT_DIR = ""

# pclint配置文件名
PCLINT_CONF_FILE = "pclintConf.yaml"

# log日志
LOG_FILE = "pclint.log"
# log demo
# err_str = "this is test {0}".format("line")
# print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), err_str))

# 存放临时内容
TMP = ""

#******************************************功能实现**************************************


def getFileEncode(fileName):
	"""
	:func: get file encoding type
	:param fileName: file name
	:return: encoding
	"""
	with open(fileName, 'rb') as f:
		data = f.read()
		return chardet.detect(data)["encoding"]



# 递归遍历删除目录
def recursiveDelDir(dirName):
	try:
		for root, dirs, files in os.walk(dirName):
			for name in files:
				os.remove(os.path.join(root, name))
			for name in dirs:
				try:
					os.rmdir(os.path.join(root, name))
				except OSError:
					recursiveDelDir(os.path.join(root, name))
		os.rmdir(dirName)
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


# 递归遍历指定目录获取文件名或目录名
def recursiveWalkDir(dirName=None, depth=None, fileType=None, pattern=None):
	"""
	:func: recursive walk the given directory to get file dict
	:param:
		dirName         absolute directory or relative directory, none for current directory
		deepth          the depth of recursive walk dir, none for the max depth
		fileType        "f" for file, "d" for directory, "l" for link , none for all
		pattern         the re pattern for file name
	:return:
		fileDict = {
			"root": "",
			"file": [],
			"dir":  [],
			"link": [],
		}
	:bug:
		(1) can not identify link file under window system
	"""
	# 获取根目录
	if dirName:
		if os.path.isdir(dirName):
			if os.path.isabs(dirName):
				root_dir = dirName
			else:
				root_dir = os.path.dirname(os.path.abspath(os.path.basename(dirName)))
		else:
			print("Error:[line.{0}] {1} param is not dir name!".format(str(sys._getframe().f_lineno), dirName))
			sys.exit()
	else:
		root_dir = os.getcwd()

	# 初始化fileDict
	file_dict = {}
	file_dict["root"] = root_dir
	dict_content = {
		"f": {
			"mode": 'stat.S_ISREG(os.stat("{_file}")[stat.ST_MODE])',
			"index": "file",
		},
		"d": {
			"mode": 'stat.S_ISDIR(os.stat("{_file}")[stat.ST_MODE])',
			"index": "dir",
		},
		"l": {
			"mode": 'stat.S_ISLNK(os.stat("{_file}")[stat.ST_MODE])',
			"index": "link",
		},
	}
	if fileType and fileType not in dict_content.keys():
		print("Error:[line.{0}] fileType param is invalid!".format(str(sys._getframe().f_lineno)))
		sys.exit()

	for fileTypeItem in dict_content:
		if not fileType or fileType == fileTypeItem:
			file_dict[dict_content[fileTypeItem]["index"]] = []

	# 递归遍历目录
	try:
		for root, dirs, files in os.walk(root_dir):
			for fileItem in files:
				for key in dict_content:
					if dict_content[key]["index"] in file_dict.keys():
						if not pattern or re.search(pattern, fileItem):
							file_dict[dict_content[key]["index"]].append(os.path.join(root, fileItem))
			for dirItem in dirs:
				if "dir" in file_dict.keys():
					file_dict["dir"].append(os.path.join(root, dirItem))
				if depth:
					if depth > 1:
						depth = depth - 1
					else:
						return file_dict
				file_dict_ret = recursiveWalkDir(os.path.join(root, dirItem), depth=depth, fileType=fileType, pattern=pattern)
				if not file_dict_ret:
					print("Error:[line.{0}] file_dict_ret is error!".format(str(sys._getframe().f_lineno)))
					sys.exit()
				for item in dict_content:
					if file_dict_ret.get(item) and file_dict.get(item):
						file_dict[item].extend(file_dict_ret[item])
		return file_dict
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


# 读取文件的内容
def readfile(filename):
	try:
		fp = open(filename, 'r', encoding=getFileEncode(filename))
		text = fp.readlines()
		fp.close()
		return text
	except IOError as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


# 比较两个文件内容
def comparefile(oldfilename, newfilename, reportFile):
	ht = difflib.HtmlDiff()

	oldResult = readfile(oldfilename)
	newResult = readfile(newfilename)

	try:
		with open(reportFile, 'a+', encoding="utf-8") as f:
			if oldResult and newResult:
				f.write(ht.make_file(oldResult, newResult))
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()

def writeIncludeLnt():
	try:
		# 写include.lnt文件
		fpInclude = open(INCLUDE_LNT_PATH, 'w', encoding="utf-8")
		if PRO_INCLUDE_DIR_LIST:
			for line in PRO_INCLUDE_DIR_LIST:
				# fpInclude.write("-i{0}{1}".format(line, os.linesep))
				fpInclude.write("-i{0}{1}".format(line, "\n"))
		fpInclude.close()
		# 将"include.lnt"写std.lnt文件
		stdLntFileTxt = readfile(STD_LNT_PATH)
		writeStdLnt = True
		if stdLntFileTxt:
			for line in stdLntFileTxt:
				# print(line)
				if line.find(INCLUDE_LNT_NAME) != -1:
					 writeStdLnt = False
					 break
		if writeStdLnt:
			fpStd = open(STD_LNT_PATH, 'a+', encoding=getFileEncode(STD_LNT_PATH))
			fpStd.write("{0}{1}".format(INCLUDE_LNT_NAME, "\n"))
			fpStd.close()
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


# 开始对所有驱动.c文件进行pclint检测
def startDrvPclint():
	try:
		for srcFile in DRVC_LIST:
			if srcFile:
				srcFile = srcFile.strip()
				print("PC-lint for {0}".format(srcFile))
				cmdStr = "{0}/lint-nt -i\"{0}\" -u std.lnt {1} > {2}".format(PCLINT_INSTALL_DIR, srcFile, TMP_RESULT_FILE)
				os.system(cmdStr)
				newResultFile = CUR_PCLINT_DRV_RESULT_DIR + os.sep + "{0}Result.txt".format(os.path.splitext(os.path.basename(srcFile))[0])
				shutil.copyfile(TMP_RESULT_FILE, newResultFile)

				# 和之前的结果文件进行对比
				if cmpObjExistFlag:
					oldResultFile = LAST_PCLINT_DRV_RESULT_DIR + os.sep + "{0}Result.txt".format(os.path.splitext(os.path.basename(srcFile))[0])
					if os.path.isfile(oldResultFile):
						comparefile(oldResultFile, newResultFile, CUR_PCLINT_DRV_REPORT_FILE)
					else:
						comparefile(TMP_RESULT_FILE, newResultFile, CUR_PCLINT_DRV_REPORT_FILE)
				# 和当前的结果文件进行对比
				else:
					comparefile(TMP_RESULT_FILE, newResultFile, CUR_PCLINT_DRV_REPORT_FILE)
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


# 开始对所有管理.c文件进行pclint检测
def startMngPclint():
	try:
		for srcFile in MNGC_LIST:
			if srcFile:
				srcFile = srcFile.strip()
				print("PC-lint for {0}".format(srcFile))
				cmdStr = "{0}/lint-nt -i\"{0}\" -u std.lnt {1} > {2}".format(PCLINT_INSTALL_DIR, srcFile, TMP_RESULT_FILE)
				# print(cmdStr)
				os.system(cmdStr)
				newResultFile = CUR_PCLINT_MNG_RESULT_DIR + os.sep + "{0}Result.txt".format(os.path.splitext(os.path.basename(srcFile))[0])
				shutil.copyfile(TMP_RESULT_FILE, newResultFile)

				# 和之前的结果文件进行对比
				if cmpObjExistFlag:
					oldResultFile = LAST_PCLINT_MNG_RESULT_DIR + os.sep + "{0}Result.txt".format(os.path.splitext(os.path.basename(srcFile))[0])
					if os.path.isfile(oldResultFile):
						comparefile(oldResultFile, newResultFile, CUR_PCLINT_MNG_REPORT_FILE)
					else:
						comparefile(TMP_RESULT_FILE, newResultFile, CUR_PCLINT_MNG_REPORT_FILE)
				# 和当前的结果文件进行对比
				else:
					comparefile(TMP_RESULT_FILE, newResultFile, CUR_PCLINT_MNG_REPORT_FILE)
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


def startPclint():
	if checkDrvFlag or checkMngFlag:
		writeIncludeLnt()

	print("************Pclint Drv File Start***************")
	if checkDrvFlag:
		startDrvPclint()
	else:
		print("No drv file need to pclint!")
	print("************Pclint Drv File End*****************")
	print("")
	print("************Pclint Mng File Start***************")
	if checkMngFlag:
		startMngPclint()
	else:
		print("No mng file need to pclint!")
	print("************Pclint Mng File End*****************")


def checkFlag(l):
	if l:
		for item in l:
			if item:
				return True
	return False


# 读取pclintConf.yaml配置文件生成全局配置环境变量
def getEnv():
	global PCLINT_INSTALL_DIR
	global PRO_INCLUDE_DIR_LIST
	global PRO_NAME
	global DRV_SRC_DIR_LIST
	global DRV_SRC_FILE_LIST
	global MNG_SRC_DIR_LIST
	global MNG_SRC_FILE_LIST
	global LAST_PCLINT_REPORT_PATH
	global CURRENT_PCLINT_REPORT_PATH
	global TMP_RESULT_FILE
	global checkDrvFlag
	global checkMngFlag
	global cmpObjExistFlag
	global PRO_LNT_PATH
	global INCLUDE_LNT_PATH
	global STD_LNT_PATH
	global CUR_PCLINT_DRV_REPORT_FILE
	global CUR_PCLINT_DRV_RESULT_DIR
	global CUR_PCLINT_MNG_REPORT_FILE
	global CUR_PCLINT_MNG_RESULT_DIR
	global LAST_PCLINT_DRV_REPORT_FILE
	global LAST_PCLINT_DRV_RESULT_DIR
	global LAST_PCLINT_MNG_REPORT_FILE
	global LAST_PCLINT_MNG_RESULT_DIR

	try:
		with open(PCLINT_CONF_FILE, mode="r", encoding=getFileEncode(PCLINT_CONF_FILE)) as f:
			conf_dict = yaml.load(f)
			# 获取pclint安装目录
			PCLINT_INSTALL_DIR = conf_dict["pclintInstall"]
			if not PCLINT_INSTALL_DIR:
				print("Error:[line.{0}] pclint install dir is null".format(str(sys._getframe().f_lineno)))
				sys.exit()

			# 获取项目名
			PRO_NAME = conf_dict["projectName"]

			# 获取项目头文件目录名
			PRO_INCLUDE_DIR_LIST = conf_dict["includeDirName"]

			# 获取待检查驱动源码相关环境变量
			DRV_SRC_DIR_LIST = conf_dict["drvSrcDirName"]
			DRV_SRC_FILE_LIST = conf_dict["drvSrcFileName"]
			checkDrvFlag = checkFlag(DRV_SRC_DIR_LIST) or checkFlag(DRV_SRC_FILE_LIST)

			# 获取待检查管理源码相关环境变量
			MNG_SRC_DIR_LIST = conf_dict["mngSrcDirName"]
			MNG_SRC_FILE_LIST = conf_dict["mngSrcFileName"]
			checkMngFlag = checkFlag(MNG_SRC_DIR_LIST) or checkFlag(MNG_SRC_FILE_LIST)

			# 获取pclint报告相关环境变量
			LAST_PCLINT_REPORT_PATH = conf_dict["lastPclintReport"]
			if LAST_PCLINT_REPORT_PATH:
				cmpObjExistFlag = True
			if conf_dict["currentPclintReport"]:
				CURRENT_PCLINT_REPORT_PATH = conf_dict["currentPclintReport"]
			TMP_RESULT_FILE = CURRENT_PCLINT_REPORT_PATH + os.sep + "tmpResult.txt"

			# 存放pclint待检查.c源码文件名
			PRO_LNT_PATH     = PCLINT_INSTALL_DIR + os.sep + PRO_LNT_NAME
			# 存放pclint检查所依赖头文件路径名
			INCLUDE_LNT_PATH = PCLINT_INSTALL_DIR + os.sep + INCLUDE_LNT_NAME
			# 调用include.lnt文件的路径名
			STD_LNT_PATH     = PCLINT_INSTALL_DIR + os.sep + STD_LNT_NAME

			# 当前pclint报告相关
			if checkDrvFlag:
				CUR_PCLINT_DRV_REPORT_FILE = CURRENT_PCLINT_REPORT_PATH + os.sep + "pclintDrvReport.html"
				CUR_PCLINT_DRV_RESULT_DIR  = CURRENT_PCLINT_REPORT_PATH + os.sep + "pclintDrvResult"
				if cmpObjExistFlag:
					# 上次pclint报告相关
					LAST_PCLINT_DRV_REPORT_FILE = LAST_PCLINT_REPORT_PATH + os.sep + "pclintDrvReport.html"
					LAST_PCLINT_DRV_RESULT_DIR  = LAST_PCLINT_REPORT_PATH + os.sep + "pclintDrvResult"
			if checkMngFlag:
				CUR_PCLINT_MNG_REPORT_FILE = CURRENT_PCLINT_REPORT_PATH + os.sep + "pclintMngReport.html"
				CUR_PCLINT_MNG_RESULT_DIR  = CURRENT_PCLINT_REPORT_PATH + os.sep + "pclintMngResult"
				if cmpObjExistFlag:
					# 上次pclint报告相关
					LAST_PCLINT_MNG_REPORT_FILE = LAST_PCLINT_REPORT_PATH + os.sep + "pclintMngReport.html"
					LAST_PCLINT_MNG_RESULT_DIR  = LAST_PCLINT_REPORT_PATH + os.sep + "pclintMngResult"
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


def makePclintReportDir():
	try:
		os.makedirs(CURRENT_PCLINT_REPORT_PATH)
		if checkDrvFlag:
			os.makedirs(CUR_PCLINT_DRV_RESULT_DIR)
		if checkMngFlag:
			os.makedirs(CUR_PCLINT_MNG_RESULT_DIR)
		f = open(TMP_RESULT_FILE, "w")
		f.close()
	except Exception as e:
		print("Error:[line.{0}] {1}".format(str(sys._getframe().f_lineno), e))
		sys.exit()


def generateDrvFileList():
	global DRVC_LIST

	if DRV_SRC_DIR_LIST:
		for dirItem in DRV_SRC_DIR_LIST:
			if dirItem:
				dirItem = dirItem.strip()
				file_dict = recursiveWalkDir(dirName=dirItem, fileType='f', pattern=".c$")
				DRVC_LIST.extend(file_dict["file"])
	if DRV_SRC_FILE_LIST:
		for fileItem in DRV_SRC_FILE_LIST:
			if fileItem:
				DRVC_LIST.append(fileItem)


def generateMngFileList():
	global MNGC_LIST

	if MNG_SRC_DIR_LIST:
		for dirItem in MNG_SRC_DIR_LIST:
			if dirItem:
				dirItem = dirItem.strip()
				file_dict = recursiveWalkDir(dirName=dirItem, fileType='f', pattern=".c$")
				MNGC_LIST.extend(file_dict["file"])

	if MNG_SRC_FILE_LIST:
		for fileItem in MNG_SRC_FILE_LIST:
			if fileItem:
				MNGC_LIST.append(fileItem)


def printSrcFile():
	print("*********drv file**********")
	if DRVC_LIST:
		for item in DRVC_LIST:
			print(item)
	print("***********end*************")

	print("*********mng file**********")
	if MNGC_LIST:
		for item in MNGC_LIST:
			print(item)
	print("***********end*************")

def prePclint():
	# 删除已存在的CURRENT_PCLINT_REPORT_PATH
	if checkDrvFlag or checkMngFlag:
		if os.path.isdir(CURRENT_PCLINT_REPORT_PATH):
			recursiveDelDir(CURRENT_PCLINT_REPORT_PATH)

	# 创建pclint报告相关目录
	if checkDrvFlag or checkMngFlag:
		makePclintReportDir()

	# 生成待检查.c源文件列表
	if checkDrvFlag:
		generateDrvFileList()

	if checkMngFlag:
		generateMngFileList()
	

def printEnv():
	print("*************printEnv*************************")
	print("PCLINT_INSTALL_DIR: ", PCLINT_INSTALL_DIR)
	print("PRO_INCLUDE_DIR_LIST: ", PRO_INCLUDE_DIR_LIST)
	print("PRO_NAME: ", PRO_NAME)
	print("DRV_SRC_DIR_LIST: ", DRV_SRC_DIR_LIST)
	print("DRV_SRC_FILE_LIST: ", DRV_SRC_FILE_LIST)
	print("MNG_SRC_DIR_LIST: ", MNG_SRC_DIR_LIST)
	print("MNG_SRC_FILE_LIST: ", MNG_SRC_FILE_LIST)
	print("LAST_PCLINT_REPORT_PATH: ", LAST_PCLINT_REPORT_PATH)
	print("CURRENT_PCLINT_REPORT_PATH: ", CURRENT_PCLINT_REPORT_PATH)
	print("checkDrvFlag: ", checkDrvFlag)
	print("checkMngFlag: ", checkMngFlag)
	print("cmpObjExistFlag: ", cmpObjExistFlag)
	print("TMP_RESULT_FILE: ", TMP_RESULT_FILE)
	print("PRO_LNT_PATH: ", PRO_LNT_PATH)
	print("INCLUDE_LNT_PATH: ", INCLUDE_LNT_PATH)
	print("STD_LNT_PATH: ", STD_LNT_PATH)
	print("CUR_PCLINT_DRV_REPORT_FILE: ", CUR_PCLINT_DRV_REPORT_FILE)
	print("CUR_PCLINT_DRV_RESULT_DIR: ", CUR_PCLINT_DRV_RESULT_DIR)
	print("CUR_PCLINT_MNG_REPORT_FILE: ", CUR_PCLINT_MNG_REPORT_FILE)
	print("CUR_PCLINT_MNG_RESULT_DIR: ", CUR_PCLINT_MNG_RESULT_DIR)
	print("LAST_PCLINT_DRV_REPORT_FILE: ", LAST_PCLINT_DRV_REPORT_FILE)
	print("LAST_PCLINT_DRV_RESULT_DIR: ", LAST_PCLINT_DRV_RESULT_DIR)
	print("LAST_PCLINT_MNG_REPORT_FILE: ", LAST_PCLINT_MNG_REPORT_FILE)
	print("LAST_PCLINT_MNG_RESULT_DIR: ", LAST_PCLINT_MNG_RESULT_DIR)
	print("*************end*************************")


def pclint():
	getEnv()
	# printEnv()
	prePclint()
	# printSrcFile()
	startPclint()

if __name__ == "__main__":
	pclint()
