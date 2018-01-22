package homework2;

public class _15210931_肖蕴明_2_Student {
	String studentNumber;
	String studentName;
	int markForMaths;
	int markForEnglish;
	int markForScience;
	
	public _15210931_肖蕴明_2_Student(String number, String name) 
		throws _15210931_肖蕴明_2_StudentNumberException {
			this.studentNumber = number;
			this.studentName = name;
			if (this.studentNumber.length() != 10 
				&& this.studentNumber.substring(0, 2) != "20")
				throw new _15210931_肖蕴明_2_StudentNumberException();
	}
	
	
	public _15210931_肖蕴明_2_Student(String number) 
			throws _15210931_肖蕴明_2_StudentNumberException {
		this.studentNumber = number;
		if (this.studentNumber.length() != 10 
			|| this.studentNumber.charAt(0) != '2' || this.studentNumber.charAt(1) != '0'
			|| this.studentNumber.charAt(2) < '0' || this.studentNumber.charAt(2) > '9'
			|| this.studentNumber.charAt(3) < '0' || this.studentNumber.charAt(3) > '9'
			|| this.studentNumber.charAt(4) < '0' || this.studentNumber.charAt(4) > '9'
			|| this.studentNumber.charAt(5) < '0' || this.studentNumber.charAt(5) > '9'
			|| this.studentNumber.charAt(6) < '0' || this.studentNumber.charAt(6) > '9'
			|| this.studentNumber.charAt(7) < '0' || this.studentNumber.charAt(7) > '9'
			|| this.studentNumber.charAt(8) < '0' || this.studentNumber.charAt(8) > '9'
			|| this.studentNumber.charAt(9) < '0' || this.studentNumber.charAt(9) > '9')
			throw new _15210931_肖蕴明_2_StudentNumberException();
	}
	
	public void addName(String name) {
		this.studentName = name;
	}
	
	public String getNumber() {
		return this.studentNumber;
	}
	
	public String getName() {
		return this.studentName;
	}
	
	public void enterMarks(int markForMaths, int markForEnglish, int markForScience) 
		throws _15210931_肖蕴明_2_SourceException {
		this.markForMaths = markForMaths;
		this.markForEnglish = markForEnglish;
		this.markForScience = markForScience;
		if (this.markForMaths < 0 || this.markForMaths > 100
			|| this.markForEnglish < 0 || this.markForEnglish > 100
			|| this.markForScience < 0 || this.markForScience > 100)
			throw new _15210931_肖蕴明_2_SourceException();
	}
	
	public int getMathsMark() {
		return this.markForMaths;
	}
	
	public int getEnglishMark() {
		return this.markForEnglish;
	}
	
	public int getScienceMark() {
		return this.markForScience;
	}
	
	public double calculateAverage() {
		return (this.markForMaths + this.markForEnglish + this.markForScience) / 3.0;
	}
	
	public String toString() {
		String s = "学号：" + this.studentNumber + '\n';
		s += "姓名：" + this.studentName + '\n';
		s += "数学成绩：" + this.markForMaths + '\n';
		s += "英语成绩：" + this.markForEnglish + '\n';
		s += "科学成绩：" + this.markForScience + '\n';
		s += "平均成绩：" + this.calculateAverage() + '\n';
		return s;
	}
}
