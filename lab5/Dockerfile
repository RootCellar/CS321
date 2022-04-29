FROM ubuntu
COPY anaconda.sh /root/
WORKDIR /root/

RUN chmod u+x anaconda.sh
RUN ./anaconda.sh -b; exit 0
RUN conda create -n main
RUN conda activate main
RUN conda install -c anaconda jupyter
CMD ["jupyter"]
