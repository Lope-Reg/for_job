cmd_/home/linux/work/linux_driver/interview_1/Module.symvers := sed 's/\.ko$$/\.o/' /home/linux/work/linux_driver/interview_1/modules.order | scripts/mod/modpost -m -a  -o /home/linux/work/linux_driver/interview_1/Module.symvers -e -i Module.symvers   -T -
