package homework5;

public class _15210931_Ф����_5_HourlyEmployee extends _15210931_Ф����_5_Employee {
	double wage;
	double hours;
	
	_15210931_Ф����_5_HourlyEmployee(String fn, String ln, String ssn, double w, double h) {
		super(fn, ln, ssn);
		this.wage = w;
		this.hours = h;
	}
	
	public double earning() {
		return (this.wage * this.hours);
	}
	
	public double getWage() {
        return wage;
    }

    public double getHours() {
        return hours;
    }

    public void setWage(double wage) {
        this.wage = wage;
    }

    public void setHours(double hours) {
        this.hours = hours;
    }

    public String toString(){
    	return  "нˮ���ͣ�ʱн\n" +
    			"����" + getFirstName() + '\n' +
                "�գ�" + getLastName() + '\n' +
                "�籣���ţ�" + getSocialSecurityNumber() + '\n' +
                "ÿ��нˮ��" + String.valueOf(earning()) + '\n';
    }
}
