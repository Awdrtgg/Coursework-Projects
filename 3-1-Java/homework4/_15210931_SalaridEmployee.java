package homework4;

public class _15210931_Ф����_4_SalaridEmployee extends _15210931_Ф����_4_Employee {
	double weeklySalary;
	
	_15210931_Ф����_4_SalaridEmployee(String fn, String ln, String ssn, double s) {
		super(fn, ln, ssn);
		this.weeklySalary = s;
	}
	
	public double earning() {
		return (4.0 * this.weeklySalary);
	}
	
	public double getWeeklySalary() {
        return weeklySalary;
    }

    public void setWeeklySalary(double weeklySalary) {
        this.weeklySalary = weeklySalary;
    }

    public String toString(){
        return  "нˮ���ͣ���н\n" +
                "����" + getFirstName() + '\n' +
                "�գ�" + getLastName() + '\n' +
                "�籣���ţ�" + getSocialSecurityNumber() + '\n' +
                "ÿ��нˮ��" + String.valueOf(earning()) + '\n';
    }
}
