from django.db import models

from projects.models import  ModelHealth
from projects.serializers import HealthProjectSerializer, ModelHealthSerializer

from rest_framework import serializers
from rest_framework import generics
from rest_framework.permissions import AllowAny
from rest_framework.response import Response

### models.py

class ModelHealth(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    user_id = models.CharField(max_length=300)
    lpg = models.FloatField()
    co = models.FloatField()
    smoke = models.FloatField()

    def __str__(self):
        return "f{self.user_id}"


##### serializers.py

class ModelHealthSerializer(serializers.ModelSerializer):
    class Meta:
        model = ModelHealth
        fields = '__all__'


### views 
class ModelHealthSerializerDataAPIView(generics.ListCreateAPIView):
    serializer_class = HealthProjectSerializer
    queryset =  ModelHealthSerializer.objects.all()
    permission_classes= (AllowAny,)
    #authentication_classes = ( BaseAuthentication,)

    @swagger_auto_schema(
            responses={
                '200': ModelHealthSerializer,
                '400': ModelHealthSerializer,
            },
            request_body=ModelHealthSerializer,
    )
    def post(self, request, *args, **kwargs):
        return super().post(request, *args, **kwargs)

    def get(self, request, *args, **kwargs):
        user_id = self.request.query_params.get('user_id', None)
        temperature = self.request.query_params.get('temperature', None)
        humidity = self.request.query_params.get('humidity', None)
        lpg = self.request.query_params.get('lpg', None)
        co = self.request.query_params.get('co', None)
        smoke = self.request.query_params.get('smoke', None)

        if temperature is None or humidity is None or user_id is None or lpg is None:
            return super().get(self, request, *args, **kwargs)
           
        map = {
            "user_id": user_id,
            "temperature":temperature,
            "humidity": humidity,
            "lpg": lpg,
            "co": co,
            "smoke": smoke
        } 

        try:
           ModelHealth(**map).save()
        except Exception as e:
            return Response(str(e))
        else:
            return Response("success")

class ModelHealthDetailAPIView(generics.RetrieveAPIView):
    serializer_class = ModelHealthSerializer
    queryset = ModelHealth.objects.all()

    
    def get(self, request, pk, format=None):
        obj = self.queryset.get(pk=pk)
        serializer = self.serializer_class(obj)
        return Response(serializer.data)

